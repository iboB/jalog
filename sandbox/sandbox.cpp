// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <jalog/DefaultLogger.hpp>
#include <jalog/Instance.hpp>
#include <jalog/Log.hpp>
#include <jalog/LogPrintf.hpp>
#include <jalog/Logger.hpp>

#include <jalog/sinks/SimpleStdioSink.hpp>
#include <jalog/sinks/SimpleOStreamSink.hpp>
#include <jalog/sinks/ColorSink.hpp>
#include <jalog/sinks/AnsiColorSink.hpp>
#include <jalog/sinks/NSLogSink.hpp>

#include <jalog/Printf.hpp>

#include <iostream>

namespace math
{

struct vec { int x, y; };

std::ostream& operator<<(std::ostream& o, const vec& v)
{
    return o << v.x << ' ' << v.y;
}

jalog::BasicStream& operator,(jalog::BasicStream& s, const vec& v)
{
    return s, '(', v.x, ';', v.y, ')';
}

}

int main()
{
    jalog::Instance jl;
    jl.setup().async()
        .add<jalog::sinks::ColorSink>()
        .add<jalog::sinks::SimpleStdioSink>()
        .add<jalog::sinks::SimpleOStreamSink>(std::cout, std::cerr)
#if defined(__APPLE__)
        .add<jalog::sinks::NSLogSink>()
#endif
    ;

    JALOG(Info, "Hello ", sizeof(jalog::ScopeDesc));


    math::vec v = {1, 2};
    JALOG(Info, "vec: ", v);

    return 0;
}
