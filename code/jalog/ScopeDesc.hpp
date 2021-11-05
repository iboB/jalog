// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include <itlib/static_vector.hpp>

#include <cstdint>
#include <string_view>

namespace jalog
{

class ScopeDesc
{
public:
    std::string_view label() const { return {labelBytes.data(), labelBytes.size()}; }
    uintptr_t id() const { return m_id; }
    intptr_t userData() const { return m_userData; }

private:
    friend class Scope;

    itlib::static_vector<char, 16> m_labelBytes;
    uintptr_t m_id; // unused by jalog. May be used by user-defined sinks
    intptr_t m_userData; // unused by jalog. May be used by user-defined sinks
};

}
