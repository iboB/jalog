// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "Severity.hpp"

#if !defined(JALOG_ENABLED)
#   define JALOG_ENABLED 1
#endif

#define I_JALOG_NOOP(...)

#if JALOG_ENABLED

#define JALOG(sev, scope, ...) \
    do { \
        if (scope.enabled(::jalog::Severity::sev)) \
            logger(scope, ::jalog::Severity::sev), __VA_ARGS__ \
    } while(false)

#else

#define JALOG I_JALOG_NOOP

#endif

