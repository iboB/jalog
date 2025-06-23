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

void Logger::setInitialLevelOverride(std::optional<Level> lvl)
{
    std::lock_guard l(m_mutex);
    m_initialLevelOverride = lvl;
}

std::optional<Level> Logger::initialLevelOverride() const
{
    std::lock_guard l(m_mutex);
    return m_initialLevelOverride;
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

    m_scopeSinks.reserve(m_sinks.size());
    for (auto& sink : m_sinks) {
        m_scopeSinks.push_back(sink.get());
    }

    for (auto scope : m_scopes)
    {
        if (!scope) continue;
        initScope(*scope);
    }
}

void Logger::initScope(Scope& scope)
{
    assert(scope.m_sinks.empty());
    scope.m_sinks = m_scopeSinks;
    scope.setLevel(m_initialLevelOverride.value_or(scope.m_initialLevel));
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

Logger::SetupDSL& Logger::SetupDSL::overrideInitialLevel(Level lvl)
{
    m_logger.setInitialLevelOverride(lvl);
    return *this;
}

Logger::SetupDSL& Logger::SetupDSL::add(SinkPtr sink)
{
    m_logger.addSink(std::move(sink));
    return *this;
}

}
