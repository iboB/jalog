// jalog
// Copyright (c) 2021-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include <splat/symbol_export.h>

#if JALOG_SINKLIB_SHARED
#   if BUILDING_JALOG_SINKLIB
#       define JALOG_SINKLIB_API SYMBOL_EXPORT
#   else
#       define JALOG_SINKLIB_API SYMBOL_IMPORT
#   endif
#else
#   define JALOG_SINKLIB_API
#endif
