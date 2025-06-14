// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "Level.hpp"
#include "ScopeDesc.hpp"

#include <itlib/span.hpp>
#include <atomic>
#include <string_view>

namespace jalog
{

class Logger;
class Sink;

class JALOG_API Scope
{
public:
    // creates a default logger scope
    explicit Scope(std::string_view label, uintptr_t id = 0, intptr_t userData = -1);

    // creates a scope for a given logger
    Scope(Logger& logger, std::string_view label, uintptr_t id = 0, intptr_t userData = -1);

    ~Scope();

    Scope(const Scope&) = delete;
    Scope& operator=(const Scope&) = delete;
    Scope(Scope&&) = delete;
    Scope& operator=(Scope&&) = delete;

    Logger& logger() const { return m_logger; }

    const ScopeDesc& desc() const { return m_desc; }

    void setLevel(Level lvl) { m_level.store(lvl, std::memory_order_release); }
    Level level() const { return m_level.load(std::memory_order_acquire); }

    bool enabled(Level lvl) const { return lvl >= level(); }

    // add an entry but don't perform the enabled check
    // assume it's performed from the outside
    void addEntryUnchecked(Level lvl, std::string_view text);

    // add entry if the level is >= than our min level
    void addEntry(Level lvl, std::string_view text) { if (enabled(lvl)) addEntryUnchecked(lvl, text); }

private:
    friend class Logger;

    std::atomic<Level> m_level = Level::Debug;

protected:
    Logger& m_logger;

    ScopeDesc m_desc;

    itlib::span<Sink*> m_sinks;
};

}
