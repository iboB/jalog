// jalog
// Copyright (c) 2021-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
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
