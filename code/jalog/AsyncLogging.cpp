// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "AsyncLogging.hpp"

namespace jalog
{

namespace impl
{
class AsyncLoggingImpl
{

};
}

AsyncLogging::AsyncLogging()
    : m_impl(new impl::AsyncLoggingImpl)
{}

AsyncLogging::~AsyncLogging() = default;

void AsyncLogging::update()
{

}

void AsyncLogging::addSink(SinkPtr sink)
{

}

void AsyncLogging::removeSink(SinkPtr sink)
{

}

void AsyncLogging::record(const Entry& entry)
{

}

}
