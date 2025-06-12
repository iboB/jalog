// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

#include "Level.hpp"
#include "PrintFlags.hpp"

#include <cstdarg>

namespace jalog
{

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

template <uint32_t Flags = PrintFlags::None>
void VPrintf(Scope& scope, Level lvl, const char* format, va_list args);

template <uint32_t Flags = PrintFlags::None>
I_JALOG_PRINTF_FMT void Printf(Scope& scope, Level lvl, _Printf_format_string_ const char* format, ...);

}
