// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#if !defined(JALOG_ENABLED)
#   define JALOG_ENABLED 1
#endif

#if JALOG_ENABLED

#include "DefaultScope.hpp"
#include "FmtPrint.hpp"

#define JALOG_FMT_SCOPE(scope, lvl, fmt, ...) \
    if ((scope).enabled(::jalog::Level::lvl)) \
        ::jalog::FmtPrint<::jalog::PrintFlags::SkipLogLevelCheck>(scope, ::jalog::Level::lvl, fmt, ##__VA_ARGS__)

#define JALOG_FMT(lvl, ...) JALOG_FMT_SCOPE(::jalog::Default_Scope, lvl, __VA_ARGS__)

#else

#define JALOG_FMT_SCOPE(...)
#define JALOG_FMT(...)

#endif
