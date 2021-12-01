// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#include <string_view>
#include <memory>

namespace jalog
{

class AsyncLogging;

class JALOG_API AsyncLoggingThread {
    // implemented in AsyncLogging.cpp
public:
    AsyncLoggingThread(AsyncLogging& source, std::string_view threadName = "jalog");
    ~AsyncLoggingThread();
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}
