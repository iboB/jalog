// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "Scope.hpp"

#include "Logger.hpp"
#include "DefaultLogger.hpp"
#include "Sink.hpp"
#include "Entry.hpp"

#include <cstring>

namespace jalog
{

Scope::Scope(Logger& logger, std::string_view lbl, uintptr_t id, intptr_t userData)
    : m_logger(logger)
{
    auto len = std::min(lbl.length(), sizeof(m_desc.m_labelBytes) - 1);
    if (len > 0) {
        std::memcpy(m_desc.m_labelBytes, lbl.data(), len);
    }
    m_desc.m_labelBytes[len] = 0;
    m_desc.m_labelLength = uint32_t(len);

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
