// jalog
// Copyright (c) 2021-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
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
