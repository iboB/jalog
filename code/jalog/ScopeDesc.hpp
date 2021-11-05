// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#include <cstdint>

namespace jalog
{

struct JALOG_API ScopeDesc
{
    std::string label;
    intptr_t userData;
};

}
