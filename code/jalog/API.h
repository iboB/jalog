// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <splat/symbol_export.h>

#if JALOG_SHARED
#   if BUILDING_JALOG
#       define JALOG_API SYMBOL_EXPORT
#   else
#       define JALOG_API SYMBOL_IMPORT
#   endif
#else
#   define JALOG_API
#endif
