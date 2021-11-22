// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "Printf.hpp"

#include <cstdio>
#include <cstdarg>

namespace jalog
{

void Printf(Scope& scope, Level lvl, const char* format, ...)
{
    if (!scope.enabled(lvl)) return;

    va_list args;

    // optimistically try to fit in a small buffer
    size_t size = 0;
    {
        va_start(args, format);
        char buf[1024];
        size = size_t(vsnprintf(buf, sizeof(buf), format, args));
        va_end(args);

        if (size <= sizeof(buf))
        {
            // success
            scope.addEntryUnchecked(lvl, std::string_view(buf, size));
            return;
        }
    }

    std::vector<char> dbuf(size + 1);
    va_start(args, format);
    size = size_t(vsnprintf(dbuf.data(), dbuf.size(), format, args));
    va_end(args);

    scope.addEntryUnchecked(lvl, std::string_view(dbuf.data(), size));
}

}
