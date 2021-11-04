// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#if !defined(JALOG_ENABLED)
#   define JALOG_ENABLED 1
#endif

#define I_JALOG_NOOP(...)

#if JALOG_ENABLED

#define JALOG(sev, scope, ...) \
    do { \
        if (scope.enabled(::jalog::severity::sev)) \
            logger(scope, ::jalog::severity::sev), __VA_ARGS__ \
    } while(false)

#else

#define JALOG I_JALOG_NOOP

#endif

namespace jalog
{


}

JALOG_API int tpl_add(int a, int b);
