// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "Level.hpp"

#include <cstdarg>

namespace jalog
{

namespace PrintfFlags {
enum Flags : uint32_t {
    None = 0,

    // skip checking the log level of the scope
    // useful when this check is already done before calling Printf
    SkipLogLevelCheck = 1,

    // trim the trailing newline from the message
    // useful when replacing existing log messages (e.g. with regular printf) which already have a newline at the end
    TrimTrailingNewline = 2,
};
}

class Scope;

#if defined(__GNUC__)
#   define I_JALOG_PRINTF_FMT __attribute__((format(printf, 3, 4)))
#   define _Printf_format_string_
#else
#   define I_JALOG_PRINTF_FMT
#   if !defined(_MSC_VER)
#       define _Printf_format_string_
#   endif
#endif

template <uint32_t Flags = PrintfFlags::None>
void VPrintf(Scope& scope, Level lvl, const char* format, va_list args);

template <uint32_t Flags = PrintfFlags::None>
I_JALOG_PRINTF_FMT void Printf(Scope& scope, Level lvl, _Printf_format_string_ const char* format, ...);

}
