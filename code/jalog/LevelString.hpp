// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "Level.hpp"

#include <string_view>

namespace jalog
{

inline constexpr std::string_view Level_toString(Level l)
{
    switch (l)
    {
    case Level::Debug: return "debug";
    case Level::Info: return "Info";
    case Level::Warning: return "Warn";
    case Level::Error: return "ERROR";
    case Level::Critical: return "!CRIT!";
    default: return "?";
    }
}

}
