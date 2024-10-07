// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <jalog/Instance.hpp>
#include <jalog/sinks/DefaultSink.hpp>
#include <jalog/Log.hpp>

int main()
{
    jalog::Instance jl;
    jl.setup().add<jalog::sinks::DefaultSink>();

    JALOG(Debug, "Perparing to greet world");
    JALOG(Info, "Hello, world");

    return 0;
}
