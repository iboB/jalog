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

#include <atomic>

namespace jalog
{

class JALOG_API Scope
{
public:
    Scope();

    bool enabled(Severity sev)
    {
        return sev >= m_minSeverity.load(std::memory_order_relaxed);
    }

private:
    // not Severity as values outside of the enum are valid
    std::atomic_uint32_t m_minSeverity = 0;
};

}
