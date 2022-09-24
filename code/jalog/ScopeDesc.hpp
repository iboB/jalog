// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <itlib/static_vector.hpp>

#include <cstdint>
#include <string_view>

namespace jalog
{

class ScopeDesc
{
    friend class Scope;

    itlib::static_vector<char, 16> m_labelBytes;
    uintptr_t m_id;

public:
    std::string_view label() const { return {m_labelBytes.data(), m_labelBytes.size()}; }

    // unused by jalog. May be used by user-defined sinks
    uintptr_t id() const { return m_id; }

    intptr_t userData; // unused by jalog. May be used by user-defined sinks
};

}
