// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <cstdint>
#include <string_view>

namespace jalog
{

class ScopeDesc
{
    friend class Scope;

    char m_labelBytes[16];
    uint32_t m_labelLength;

    uintptr_t m_id;

public:
    // label is zero-terminated just in case
    std::string_view label() const { return {m_labelBytes, m_labelLength}; }

    const char* labelCStr() const { return m_labelBytes; }

    // unused by jalog. May be used by user-defined sinks
    uintptr_t id() const { return m_id; }

    intptr_t userData; // unused by jalog. May be used by user-defined sinks
};

}
