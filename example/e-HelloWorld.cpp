// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <jalog/Instance.hpp>
#include <jalog/sinks/ColorSink.hpp>
#include <jalog/Log.hpp>

int main()
{
    jalog::Instance jl;
    jl.setup().add<jalog::sinks::ColorSink>();

    JALOG(Debug, "Perparing to greet world");
    JALOG(Info, "Hello, world");

    return 0;
}
