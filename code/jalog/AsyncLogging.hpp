// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#include "SinkPtr.hpp"
#include "Sink.hpp"

namespace jalog
{

namespace impl { class AsyncLoggingImpl; }
class AsyncLoggingThread;

class JALOG_API AsyncLogging final : public Sink
{
public:
    AsyncLogging(size_t initialTextBufferSize = 4096);
    ~AsyncLogging();

    // if you don't launch the associated thread
    // you are responsible to call update manually
    // update calls can't be concurrent! (though they don't necessarily need to happen on the same thread)
    void update();

    // safe on any thread and concurrency
    void add(SinkPtr sink);
    template <typename S>
    void add() { return add(std::make_shared<S>()); }
    void remove(const Sink* sink);
    void remove(const SinkPtr& sink) { remove(sink.get()); }

    virtual void record(const Entry& entry) override;

private:
    friend class AsyncLoggingThread;
    std::unique_ptr<impl::AsyncLoggingImpl> m_impl;
};

}
