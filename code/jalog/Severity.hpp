// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <cstdint>

namespace jalog
{

enum class Severity : uint32_t
{
    Debug,
    Info,
    Warning,
    Error,
    Fatal,
};

}
