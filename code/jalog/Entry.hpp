// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#include "Level.hpp"
#include "ScopeDesc.hpp"

#include <string_view>
#include <chrono>

namespace jalog
{

struct Entry
{
    ScopeDesc scope;
    Level level;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    std::string_view text;
};

}
