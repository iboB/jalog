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

    // double-buffer executing tasks
    std::vector<Task> m_executingTasks;

    void swapBuffers()
    {
        assert(m_executingTasks.empty());
        m_executingTasks.swap(m_taskQueue);
    }

    void executeTasks()
    {
        for (auto& task : m_executingTasks)
        {
            std::visit([this](auto& t) { t(*this); }, task);
        }
        m_executingTasks.clear();
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
    void pushTask(T&& t)
    {
        m_taskQueue.emplace_back(std::forward<T>(t));
    }
};
}

using AL = impl::AsyncLoggingImpl;

AsyncLogging::AsyncLogging()
    : m_impl(new AL)
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
