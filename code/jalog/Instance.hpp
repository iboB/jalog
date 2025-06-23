// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
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
        explicit SetupDSL(Instance& i) : m_instance(i) {}
        ~SetupDSL();
        SetupDSL& overrideInitialLevel(Level lvl);
        SetupDSL& add(SinkPtr sink);
        template <typename S, typename... Args>
        SetupDSL& add(Args&&... args) { return add(std::make_shared<S>(std::forward<Args>(args)...)); }
        SetupDSL& async();
    private:
        Instance& m_instance;
    };

    SetupDSL setup() { return SetupDSL(*this); }

    Logger& logger() { return m_logger; }

#if !JALOG_NO_BUILTIN_ASYNC
    AsyncLogging* asyncLogging() { return m_asyncLogging.get(); }
#endif

private:
    Logger& m_logger;
#if !JALOG_NO_BUILTIN_ASYNC
    std::shared_ptr<AsyncLogging> m_asyncLogging;
    std::unique_ptr<AsyncLoggingThread> m_asyncLoggingThread;
#endif
};

}
