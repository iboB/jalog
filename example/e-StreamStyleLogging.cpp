// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <jalog/Instance.hpp>
#include <jalog/sinks/DefaultSink.hpp>
#include <jalog/LogStream.hpp>

int main()
{
    jalog::Instance jl;
    jl.setup().add<jalog::sinks::DefaultSink>();

    auto log = JALOG_STREAM(Info);

    log << "Hello";
    log << ", stream world!" << jalog::endl;

    log << jalog::Level::Debug << "jalog::Stream was used";

    return 0;
}
