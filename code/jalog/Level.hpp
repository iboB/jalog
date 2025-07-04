// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#include <cstdint>

namespace jalog
{

enum class Level : uint32_t
{
    Lowest,
    Debug = Lowest,
    Info,
    Warning,
    Error,
    Critical,
    Off
};

}
