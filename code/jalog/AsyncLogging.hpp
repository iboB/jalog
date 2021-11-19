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

namespace impl
{
class AsyncLoggingImpl;
}

class JALOG_API AsyncLogging final : public Sink
{
public:
    AsyncLogging();
    ~AsyncLogging();

    // if you don't launch the associated thread
    // you are responsible to call update manually
    void update();

    // safe on any thread
    void add(SinkPtr sink);
    template <typename S>
    void add() { return add(std::make_shared<S>()); }
    void remove(const Sink* sink);
    void remove(const SinkPtr& sink) { remove(sink.get()); }

    virtual void record(const Entry& entry) override;

private:
    std::unique_ptr<impl::AsyncLoggingImpl> m_impl;
};

}
