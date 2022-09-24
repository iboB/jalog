// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "Logger.hpp"
#include "DefaultLogger.hpp"
#include "Scope.hpp"
#include "DefaultScope.hpp"
#include "Sink.hpp"

#include <itlib/qalgorithm.hpp>

#include <cassert>

namespace jalog
{

Logger& DefaultLogger()
{
    static Logger l;
    return l;
}

Scope Default_Scope(std::string_view{});

Logger::Logger() = default;
Logger::~Logger() = default;

void Logger::setDefaultLevel(Level lvl)
{
    std::lock_guard l(m_mutex);
    m_defaultLevel = lvl;
}

Level Logger::defaultLevel() const
{
    std::lock_guard l(m_mutex);
    return m_defaultLevel;
}

void Logger::flush()
{
    std::lock_guard l(m_mutex);
    if (!m_initialized) return; // nothing to flush
    for (auto& s : m_sinks)
    {
        s->flush();
    }
}

void Logger::addSink(SinkPtr sink)
{
    std::lock_guard l(m_mutex);
    assert(!m_initialized);
    if (m_initialized) return; // defensive: Don't crash
    m_sinks.push_back(std::move(sink));
}

void Logger::initialize()
{
    std::lock_guard l(m_mutex);
    assert(!m_initialized);
    if (m_initialized) return; // defensive: Don't crash

    m_initialized = true;

    for (auto scope : m_scopes)
    {
        if (!scope) continue;
        initScope(*scope);
    }
}

void Logger::initScope(Scope& scope)
{
    scope.setLevel(m_defaultLevel);
    scope.m_sinks.clear();
    scope.m_sinks.reserve(m_sinks.size());
    for (auto& s : m_sinks)
    {
        scope.m_sinks.emplace_back(s.get());
    }
}

void Logger::registerScope(Scope& scope)
{
    std::lock_guard l(m_mutex);

    if (m_initialized)
    {
        // if the we're initialized, also initialize scope
        initScope(scope);
    }
    else
    {
        // otherwise disable scope until we're initialized (if we ever are)
        scope.setLevel(Level::Off);
    }

    // place scope into sparse vector registry
    auto slot = itlib::pfind(m_scopes, nullptr);
    if (!slot)
    {
        slot = &m_scopes.emplace_back();
    }
    *slot = &scope;
}

void Logger::unregisterScope(Scope& scope)
{
    std::lock_guard l(m_mutex);
    // free slot in sparse vector registry
    auto slot = itlib::pfind(m_scopes, &scope);
    assert(slot); // bug! Scope must be registered
    *slot = nullptr;
}

Logger::SetupDSL::SetupDSL(Logger& l) : m_logger(l) {};

Logger::SetupDSL::~SetupDSL()
{
    m_logger.initialize();
}

Logger::SetupDSL& Logger::SetupDSL::defaultLevel(Level lvl)
{
    m_logger.setDefaultLevel(lvl);
    return *this;
}

Logger::SetupDSL& Logger::SetupDSL::add(SinkPtr sink)
{
    m_logger.addSink(std::move(sink));
    return *this;
}

}
