// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#include "Scope.hpp"

namespace jalog
{

#if defined(__GNUC__)
#   define I_JALOG_PRINTF_FMT __attribute__((format(printf, 3, 4)))
#   define _Printf_format_string_
#else
#   define I_JALOG_PRINTF_FMT
#   if !defined(_MSC_VER)
#       define _Printf_format_string_
#   endif
#endif

JALOG_API I_JALOG_PRINTF_FMT void Printf(Scope& scope, Level lvl, _Printf_format_string_ const char* format, ...);

}
