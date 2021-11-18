// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "Scope.hpp"

#include "Logger.hpp"
#include "DefaultLogger.hpp"
#include "Sink.hpp"

#include <cstring>

namespace jalog
{

Scope::Scope(Logger& logger, std::string_view label, uintptr_t id, intptr_t userData)
    : m_logger(logger)
{
    std::memcpy(
        m_desc.m_labelBytes.data(),
        label.data(),
        std::min(label.size(), m_desc.m_labelBytes.capacity()));

    m_desc.m_id = id;
    m_desc.userData = userData;

    m_logger.registerScope(*this);
}

Scope::Scope(std::string_view label, uintptr_t id, intptr_t userData)
    : Scope(DefaultLogger(), label, id, userData)
{}

Scope::~Scope()
{
    m_logger.unregisterScope(*this);
}

void Scope::addEntryUnchecked(Level lvl, std::string_view text)
{
    Entry e;
    e.scope = m_desc;
    e.level = lvl;
    e.timestamp = std::chrono::system_clock::now();
    e.text = text;
    for (auto s : m_sinks)
    {
        s->record(e);
    }
}

}
