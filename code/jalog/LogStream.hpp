// jalog
// Copyright (c) 2021-2022 Borislav Stanimirov
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
#include "Stream.hpp"

#define JALOG_STREAM_SCOPE(scope, lvl) ::jalog::Stream(scope, ::jalog::Level::lvl)
#define JALOG_STREAM(lvl) JALOG_STREAM_SCOPE(::jalog::Default_Scope, lvl)

#else

#include "NoopStream.hpp"

#define JALOG_STREAM_SCOPE(...) ::jalog::NoopStream()
#define JALOG_STREAM(...) ::jalog::NoopStream()

#endif

