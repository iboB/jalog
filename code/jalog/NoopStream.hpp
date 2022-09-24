// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

namespace jalog
{

// do-nothing stream for cases where logging is disabled at compile time
class NoopStream
{
public:
    template <typename T>
    NoopStream& operator<<(const T&) { return *this; }
};

}
