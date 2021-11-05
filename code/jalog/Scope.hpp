// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#include "Severity.hpp"
#include "ScopeDesc.hpp"

#include <atomic>
#include <string_view>

namespace jalog
{

class JALOG_API Scope
{
public:
    Scope(std::string_view label, uintptr_t id = 0, intptr_t userData = -1);

    const ScopeDesc& desc() const { return m_desc; }

    bool enabled(Severity sev)
    {
        return sev >= m_minSeverity.load(std::memory_order_relaxed);
    }

    void addEntry(Severity sev, std::string_view text);

private:
    // not Severity as values outside of the enum are valid
    std::atomic_uint32_t m_minSeverity = 0;

    ScopeDesc m_desc;
};

}
