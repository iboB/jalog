// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#include "Level.hpp"
#include "SinkPtr.hpp"

namespace jalog
{

class Logger;
class AsyncLogging;
class AsyncLoggingThread;

class JALOG_API Instance {
public:
    Instance(); // DefaultLogger instance
    explicit Instance(Logger& l);
    ~Instance();

    class JALOG_API SetupDSL {
    public:
        SetupDSL(Instance& i) : m_instance(i) {}
        ~SetupDSL();
        SetupDSL& defaultLevel(Level lvl);
        SetupDSL& add(SinkPtr sink);
        template <typename S, typename... Args>
        SetupDSL& add(Args&&... args) { return add(std::make_shared<S>(std::forward<Args>(args)...)); }
        SetupDSL& async();
    private:
        Instance& m_instance;
    };

    SetupDSL setup() { return SetupDSL(*this); }

    Logger& logger() { return m_logger; }

    AsyncLogging* asyncLogging() { return m_asyncLogging.get(); }

private:
    Logger& m_logger;
    std::shared_ptr<AsyncLogging> m_asyncLogging;
    std::unique_ptr<AsyncLoggingThread> m_asyncLoggingThread;
};

}
