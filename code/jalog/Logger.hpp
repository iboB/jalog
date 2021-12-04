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

#include <vector>
#include <mutex>

namespace jalog
{

class Sink;
class Scope;

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
        SetupDSL& defaultLevel(Level lvl);
        SetupDSL& add(SinkPtr sink);
        template <typename S>
        SetupDSL& add() { return add(std::make_shared<S>()); }
    private:
        Logger& m_logger;
    };

    SetupDSL directSetup() { return SetupDSL(*this); }

    // default level for newly created scopes
    // scopes set their own level afterwards
    void setDefaultLevel(Level lvl);
    Level defaultLevel() const;

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

private:
    mutable std::mutex m_mutex;

    // setup
    void addSink(SinkPtr sink);
    void initialize();
    std::vector<SinkPtr> m_sinks;
    bool m_initialized = false;

    // scope registry
    Level m_defaultLevel = Level::Debug;
    friend class Scope;
    void registerScope(Scope& scope);
    void unregisterScope(Scope& scope);
    std::vector<Scope*> m_scopes; // sparse vector

    void initScope(Scope& scope);
};

}
