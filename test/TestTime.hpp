// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <chrono>

uint64_t tonano(std::chrono::system_clock::time_point t)
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();
}

uint64_t nanotime()
{
    return tonano(std::chrono::system_clock::now());
}
