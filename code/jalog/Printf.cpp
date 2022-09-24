// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "Printf.hpp"

#include "Scope.hpp"

#include <cstdio>
#include <cstdarg>

namespace jalog
{

void VPrintfUnchecked(Scope& scope, Level lvl, const char* format, va_list args)
{
    va_list acopy;
    va_copy(acopy, args);

    // optimistically try to fit in a small buffer
    size_t size = 0;
    {
        char buf[1024];
        size = size_t(vsnprintf(buf, sizeof(buf), format, args));

        if (size <= sizeof(buf))
        {
            // success
            scope.addEntryUnchecked(lvl, std::string_view(buf, size));
            return;
        }
    }

    std::vector<char> dbuf(size + 1);
    size = size_t(vsnprintf(dbuf.data(), dbuf.size(), format, acopy));

    scope.addEntryUnchecked(lvl, std::string_view(dbuf.data(), size));
}

void VPrintf(Scope& scope, Level lvl, const char* format, va_list args)
{
    if (!scope.enabled(lvl)) return;
    VPrintfUnchecked(scope, lvl, format, args);
}

void Printf(Scope& scope, Level lvl, const char* format, ...)
{
    if (!scope.enabled(lvl)) return;

    va_list args;
    va_start(args, format);
    VPrintfUnchecked(scope, lvl, format, args);
    va_end(args);
}

void PrintfUnchecked(Scope& scope, Level lvl, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    VPrintfUnchecked(scope, lvl, format, args);
    va_end(args);
}

}
