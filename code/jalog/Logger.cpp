// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "Logger.hpp"
#include "Scope.hpp"
#include "Sink.hpp"

#include <itlib/qalgorithm.hpp>

#include <cassert>

namespace jalog
{

Logger::Logger() = default;
Logger::~Logger() = default;

void Logger::setInitialLevelOverride(Level lvl)
{
    std::lock_guard l(m_mutex);
    m_initialLevelOverride = lvl;
}

Level Logger::initialLevelOverride() const
{
    std::lock_guard l(m_mutex);
    return m_initialLevelOverride;
}

void Logger::flush()
{
    std::lock_guard l(m_mutex);
    if (m_initState == InitState::None) return; // nothing to flush
    for (auto& s : m_sinks)
    {
        s->flush();
    }
}

void Logger::addSink(SinkPtr sink)
{
    std::lock_guard l(m_mutex);
    assert(m_initState == InitState::None);
    if (m_initState != InitState::None) return; // defensive: Don't crash
    m_sinks.push_back(std::move(sink));
}

void Logger::prepareUserInit() {
    std::lock_guard l(m_mutex);
    assert(m_initState != InitState::User); // double user init?
    if (m_initState == InitState::User) return; // defensive: Don't crash
    if (m_initState == InitState::None) return; // nothing to do

    m_initState = InitState::None;

    // reset scopes to uninitialized state
    for (auto scope : m_scopes) {
        if (!scope) continue;
        scope->setLevel(Level::Off);
        scope->m_sinks = {};
    }

    m_scopeSinks.clear();
    m_sinks.clear();
}

void Logger::initialize(InitState initState)
{
    assert(initState != InitState::None);

    std::lock_guard l(m_mutex);
    assert(m_initState == InitState::None);
    if (m_initState != InitState::None) return; // defensive: Don't crash

    m_initState = initState;

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

    // only override if the scope's initial level is lower than the override
    if (m_initialLevelOverride > scope.m_initialLevel) {
        scope.setLevel(m_initialLevelOverride);
    }
    else {
        scope.setLevel(scope.m_initialLevel);
    }
}

void Logger::registerScope(Scope& scope)
{
    std::lock_guard l(m_mutex);

    if (m_initState != InitState::None)
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

Logger::SetupDSL::SetupDSL(Logger& l) : m_logger(l) {
    m_logger.prepareUserInit();
}

Logger::SetupDSL::~SetupDSL() {
    m_logger.initialize(InitState::User);
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
