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

#include <chrono>

namespace jalog
{

struct ScopeDesc;

class JALOG_API Sink
{
public:
    virtual ~Sink();

    virtual void record(
        ScopeDesc& scope,
        Severity severity,
        std::chrono::time_point<std::chrono::system_clock> timestamp,
        std::string_view msg) = 0;
};

}
