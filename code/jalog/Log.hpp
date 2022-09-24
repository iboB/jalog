// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#if !defined(JALOG_ENABLED)
#   define JALOG_ENABLED 1
#endif

#if JALOG_ENABLED

#include "DefaultScope.hpp"
#include "BasicStream.hpp"

#define JALOG_SCOPE(scope, lvl, ...) \
    if (scope.enabled(::jalog::Level::lvl)) \
        ::jalog::BasicStream(scope, ::jalog::Level::lvl), __VA_ARGS__, ::jalog::endl

#define JALOG(lvl, ...) JALOG_SCOPE(::jalog::Default_Scope, lvl, __VA_ARGS__)

#else

#define JALOG_SCOPE(...)
#define JALOG(...)

#endif

