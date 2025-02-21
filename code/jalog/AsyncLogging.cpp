// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "AsyncLogging.hpp"
#include "AsyncLoggingThread.hpp"

#include "Entry.hpp"

#include <itlib/pod_vector.hpp>
#include <itlib/qalgorithm.hpp>

#include <vector>
#include <variant>
#include <mutex>
#include <cassert>
#include <future>
#include <atomic>
#include <thread>
#include <condition_variable>

namespace jalog
{

namespace impl
{
class AsyncLoggingImpl
{
public:
    AsyncLoggingImpl(size_t initialTextBufferSize)
    {
        m_textBuffer.reserve(initialTextBufferSize);
        m_loggingTextBuffer.reserve(initialTextBufferSize);
    }

    // pack raw pointers along with the sinks to skip
    // a potential control block hop of shared_ptr::->
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
        LogEntry(const Entry& e) : entry(e) {}
        Entry entry;
        int ibegin = 0; // index within loggingBuffer with which to rebuild entry.text
        void operator()(AsyncLoggingImpl& l)
        {
            // "hacky" step 2
            // our entry is currently invalid, though the size is correct
            // rebuild it via the ibegin index (and its valid size) from pointers within loggingBuffer

            entry.text = std::string_view(l.m_loggingTextBuffer.data() + ibegin, entry.text.size());

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

    struct Synchronize {
        std::promise<void> promise;
        void operator()(AsyncLoggingImpl&)
        {
            promise.set_value();
        }
    };

    using Task = std::variant<LogEntry, AddSink, RemoveSink, Synchronize>;

    std::mutex m_tasksMutex;
    std::condition_variable m_tasksCv;
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

    void update(std::unique_lock<std::mutex>& lock) {
        assert(lock.owns_lock());
        swapBuffers();
        lock.unlock();
        executeTasks();
    }

    void update()
    {
        std::unique_lock lock(m_tasksMutex);
        m_tasksCv.wait(lock, [this] { return !m_taskQueue.empty(); });
        update(lock);
    }

    void finalize() {
        std::unique_lock lock(m_tasksMutex);
        update(lock);
    }

    template <typename T>
    void pushTaskL(T&& t)
    {
        m_taskQueue.emplace_back(std::forward<T>(t));
    }

    void pushTaskL(LogEntry le)
    {
        auto& addedText = le.entry.text;

        // set the start index of the entry
        // this is our "hacky" step 1
        // note that we can't just set entry text to point inside the buffer here because it grows
        // when it grows it can get reallocated and our pointers will become invalid
        // that's why we keep the starting index and will rebuild the text in step 2
        le.ibegin = int(m_textBuffer.size());

        // copy the text into our text buffer
        m_textBuffer.insert(m_textBuffer.end(), addedText.begin(), addedText.end());
        m_textBuffer.push_back(0); // also zero-terminate string to be nice

        // now add the modified task
        m_taskQueue.emplace_back(le);
    }

    template <typename T>
    void pushTask(T&& t)
    {
        {
            std::lock_guard l(m_tasksMutex);
            pushTaskL(std::forward<T>(t));
        }
        m_tasksCv.notify_one();
    }

    void flush() {
        Synchronize sync;
        auto f = sync.promise.get_future();
        pushTask(std::move(sync));
        f.wait();
    }

    void wakeUp() {
        pushTask(Synchronize{});
    }
};
} // namespace impl

using AL = impl::AsyncLoggingImpl;

AsyncLogging::AsyncLogging(size_t initialTextBufferSize)
    : m_impl(new AL(initialTextBufferSize))
{
}

AsyncLogging::~AsyncLogging() = default;

void AsyncLogging::update()
{
    m_impl->update();
}

void AsyncLogging::add(SinkPtr sink)
{
    m_impl->pushTask(AL::AddSink{ std::move(sink) });
}

void AsyncLogging::remove(const Sink* sink)
{
    m_impl->pushTask(AL::RemoveSink{ sink });
}

void AsyncLogging::record(const Entry& entry)
{
    m_impl->pushTask(AL::LogEntry(entry));
}

void AsyncLogging::flush()
{
    m_impl->flush();
}

// thread

class AsyncLoggingThread::Impl
{
public:
    AL& m_al;

    Impl(AL& al, std::string_view threadName)
        : m_al(al)
    {
        m_running.test_and_set();

        m_thread = std::thread([this, &al, name = std::string(threadName)] {
            // TODO: set thread name

            while (m_running.test_and_set(std::memory_order_acquire)) {
                al.update();
            }
            al.finalize();
        });
    }

    ~Impl() {
        m_running.clear(std::memory_order_release);
        m_al.wakeUp();
        m_thread.join();
    }

    std::thread m_thread;
    std::atomic_flag m_running = ATOMIC_FLAG_INIT;
};

AsyncLoggingThread::AsyncLoggingThread(AsyncLogging& source, std::string_view threadName)
    : m_impl(new Impl(*source.m_impl, threadName))
{}

AsyncLoggingThread::~AsyncLoggingThread() = default;

}
