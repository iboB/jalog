// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "AsyncLogging.hpp"
#include "AsyncLoggingThread.hpp"

#include "Entry.hpp"

#include <xec/ExecutorBase.hpp>
#include <xec/ThreadExecution.hpp>

#include <itlib/pod_vector.hpp>
#include <itlib/qalgorithm.hpp>
#include <vector>
#include <variant>
#include <mutex>
#include <cassert>

namespace jalog
{

namespace impl
{
class AsyncLoggingImpl final : public xec::ExecutorBase
{
public:
    AsyncLoggingImpl(size_t initialTextBufferSize)
    {
        m_textBuffer.reserve(initialTextBufferSize);
        m_loggingTextBuffer.reserve(initialTextBufferSize);
    }

    // pack raw pointers along with the sinks to skip
    // the control block hop of shared_ptr::->
    struct SinkData
    {
        SinkData(SinkPtr p) : ptr(std::move(p)), sink(ptr.get()) {}
        SinkPtr ptr;
        Sink* sink;
    };

    std::vector<SinkData> m_sinks;

    // tasks
    struct LogEntry
    {
        Entry entry;
        void operator()(AsyncLoggingImpl& l)
        {
            // "hacky" step 2
            // our entry's now has indices instead of pointers
            // reinstate them as pointers within loggingBuffer
            const auto indexBegin = reinterpret_cast<uintptr_t>(entry.text.data());

            entry.text = std::string_view(l.m_loggingTextBuffer.data() + indexBegin, entry.text.size());

            for (auto& sd : l.m_sinks)
            {
                sd.sink->record(entry);
            }
        }
    };

    struct AddSink
    {
        SinkPtr sink;
        void operator()(AsyncLoggingImpl& l)
        {
            l.m_sinks.emplace_back(std::move(sink));
        }
    };

    struct RemoveSink
    {
        const Sink* sink;
        void operator()(AsyncLoggingImpl& l)
        {
            itlib::erase_all_if(l.m_sinks, [this](const SinkData& sd) { return sd.sink == sink; });
        }
    };

    using Task = std::variant<LogEntry, AddSink, RemoveSink>;

    std::mutex m_tasksMutex;
    std::vector<Task> m_taskQueue;
    itlib::pod_vector<char> m_textBuffer;

    // double-buffer executing tasks and text buffer
    std::vector<Task> m_executingTasks;
    itlib::pod_vector<char> m_loggingTextBuffer;

    void swapBuffers()
    {
        assert(m_executingTasks.empty() && m_loggingTextBuffer.empty());
        m_executingTasks.swap(m_taskQueue);
        m_loggingTextBuffer.swap(m_textBuffer);
    }

    void executeTasks()
    {
        for (auto& task : m_executingTasks)
        {
            std::visit([this](auto& t) { t(*this); }, task);
        }
        m_executingTasks.clear();
        m_loggingTextBuffer.clear();
    }

    virtual void update() override
    {
        m_tasksMutex.lock();
        swapBuffers();
        m_tasksMutex.unlock();
        executeTasks();
    }

    virtual void finalize() override
    {
        update();
    }

    template <typename T>
    void pushTaskL(T&& t)
    {
        m_taskQueue.emplace_back(std::forward<T>(t));
    }

    void pushTaskL(LogEntry le)
    {
        auto& addedText = le.entry.text;

        const auto addedTextStart = m_textBuffer.size();
        const auto addedTextSize = addedText.length();

        // for entries we must also copy the text into our text buffer
        m_textBuffer.insert(m_textBuffer.end(), addedText.begin(), addedText.end());
        m_textBuffer.push_back(0); // also zero-terminate string to be nice

        // now we do "hacky" step 1
        // we set the entry's text to point to zero-offset pointers
        // next, when we log the actual entry, we will update them again to point in the buffer
        // in "hacky" step 2
        // note that we can't just set them to point inside the buffer here because it grows
        // when it grows it can get reallocated and our pointers will become invalid
        // that's why instead of adding several helper types, we just use the internal
        // string_view pointer as indices
        char* indexBegin = nullptr;
        indexBegin += addedTextStart;
        addedText = std::string_view(indexBegin, addedTextSize);

        // now add the modified task
        m_taskQueue.emplace_back(le);
    }

    template <typename T>
    void pushTask(T&& t)
    {
        std::lock_guard l(m_tasksMutex);
        pushTaskL(std::forward<T>(t));
        wakeUpNow();
    }
};
}

using AL = impl::AsyncLoggingImpl;

AsyncLogging::AsyncLogging(size_t initialTextBufferSize)
    : m_impl(new AL(initialTextBufferSize))
{}

AsyncLogging::~AsyncLogging() = default;

void AsyncLogging::update()
{
    m_impl->update();
}

void AsyncLogging::add(SinkPtr sink)
{
    m_impl->pushTask(AL::AddSink{std::move(sink)});
}

void AsyncLogging::remove(const Sink* sink)
{
    m_impl->pushTask(AL::RemoveSink{sink});
}

void AsyncLogging::record(const Entry& entry)
{
    m_impl->pushTask(AL::LogEntry{entry});
}

// thread

class AsyncLoggingThread::Impl
{
public:
    Impl(AL& al, std::string_view threadName)
        : m_execution(al)
    {
        std::optional<std::string_view> name;
        if (!threadName.empty()) name = threadName;
        m_execution.launchThread(name);
    }

    xec::ThreadExecution m_execution;
};

AsyncLoggingThread::AsyncLoggingThread(AsyncLogging& source, std::string_view threadName)
    : m_impl(new Impl(*source.m_impl, threadName))
{}

AsyncLoggingThread::~AsyncLoggingThread() = default;

}
