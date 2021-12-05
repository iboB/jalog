// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "../Sink.hpp"
#include "../Entry.hpp"

#include <cstdio>

#include <itlib/time_t.hpp>

namespace jalog::sinks
{

class SimpleFILESink
{
public:
    SimpleStreamSink(FILE* out, FILE* err = nullptr)
        : m_out(out)
        , m_err(err ? err : out)
    {}

private:
    FILE* m_out;
    FILE* m_err;
}

}
