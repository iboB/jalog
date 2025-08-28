// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "Level.hpp"
#include "SinkPtr.hpp"

#include <vector>
#include <mutex>

namespace jalog
{

class Sink;
class Scope;
class Instance;

class JALOG_API Logger
{
public:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    class JALOG_API SetupDSL
    {
    public:
        explicit SetupDSL(Logger& l);
        ~SetupDSL();
        SetupDSL& overrideInitialLevel(Level lvl);
        SetupDSL& add(SinkPtr sink);
        template <typename S, typename... Args>
        SetupDSL& add(Args&&... args) { return add(std::make_shared<S>(std::forward<Args>(args)...)); }
    private:
        Logger& m_logger;
    };

    SetupDSL directSetup() { return SetupDSL(*this); }

    // override level for newly created scopes ONLY IF the scope's own initial level is lower than this
    // scopes set their own level afterwards
    void setInitialLevelOverride(Level lvl);
    Level initialLevelOverride() const;

    // iterate scopes with a functor
    // if the functor returns true, stops the iteration
    template <typename F>
    void foreachScope(F&& f)
    {
        std::lock_guard l(m_mutex);
        for (auto scope : m_scopes)
        {
            if (!scope) continue;
            if (f(*scope)) break;
        }
    }

    // flush sinks
    // can be used in signal handlers or in other cases where
    // abnormal termination is imminent and there is risk of losing log messages
    void flush();

protected:
    enum class InitState {
        None,
        System,
        User,
    };

    void addSink(SinkPtr sink);
    void initialize(InitState initState);
    void prepareUserInit();

private:
    mutable std::mutex m_mutex;

    // Sinks and initialization
    friend class Instance;
    std::vector<SinkPtr> m_sinks;
    std::vector<Sink*> m_scopeSinks; // for scope initialization
    InitState m_initState = InitState::None;

    // scope registry
    Level m_initialLevelOverride = Level::Lowest;
    friend class Scope;
    void registerScope(Scope& scope);
    void unregisterScope(Scope& scope);
    std::vector<Scope*> m_scopes; // sparse vector

    void initScope(Scope& scope);
};

}
