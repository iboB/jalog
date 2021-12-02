// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
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
}

}
