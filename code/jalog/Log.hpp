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

#define I_JALOG_NOOP(...)

#if JALOG_ENABLED

#include "DefaultScope.hpp"
#include "Stream.hpp"

#define JALOG_SCOPE(scope, lvl, ...) \
    if (scope.enabled(::jalog::Level::lvl)) \
        ::jalog::Stream(scope, ::jalog::Level::lvl), __VA_ARGS__

#define JALOG(lvl, ...) JALOG_SCOPE(::jalog::Default_Scope, lvl, __VA_ARGS__)

#else

#define JALOG_SCOPE I_JALOG_NOOP
#define JALOG I_JALOG_NOOP

#endif

