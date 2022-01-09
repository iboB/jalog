// jalog
// Copyright (c) 2021-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <jalog/Instance.hpp>
#include <jalog/sinks/ColorSink.hpp>
#include <jalog/DefaultScope.hpp>

#include <sstream>

int main()
{
    jalog::Instance jl;
    jl.setup().add<jalog::sinks::ColorSink>();

    std::ostringstream sout;
    sout << "Producing a custom string " << 34 << ' ' << 4.2;
    jalog::Default_Scope.addEntry(jalog::Level::Info, sout.str());

    return 0;
}
