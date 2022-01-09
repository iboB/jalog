// jalog
// Copyright (c) 2021-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <jalog/Instance.hpp>
#include <jalog/sinks/ColorSink.hpp>
#include <jalog/LogStream.hpp>

int main()
{
    jalog::Instance jl;
    jl.setup().add<jalog::sinks::ColorSink>();

    auto log = JALOG_STREAM(Info);

    log << "Hello";
    log << ", stream world!" << jalog::endl;

    log << jalog::Level::Debug << "jalog::Stream was used";

    return 0;
}
