// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#if !defined(JALOG_ENABLED)
#   define JALOG_ENABLED 1
#endif

#if JALOG_ENABLED

#include "DefaultScope.hpp"
#include "Printf.hpp"

#define JALOG_PRINTF_SCOPE(scope, lvl, fmt, ...) \
    if (scope.enabled(::jalog::Level::lvl)) \
        ::jalog::PrintfUnchecked(scope, ::jalog::Level::lvl, fmt, ##__VA_ARGS__)

#define JALOG_PRINTF(lvl, ...) JALOG_PRINTF_SCOPE(::jalog::Default_Scope, lvl, __VA_ARGS__)

#else

#define JALOG_PRINTF_SCOPE(...)
#define JALOG_PRINTF(...)

#endif

