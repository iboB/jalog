// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <jalog/DefaultScope.hpp>

#include <sstream>

int main() {
    std::ostringstream sout;
    sout << "Producing a custom string " << 34 << ' ' << 4.2;
    jalog::Default_Scope.addEntry(jalog::Level::Info, sout.str());

    return 0;
}
