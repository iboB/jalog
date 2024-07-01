// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#if defined(_WIN32)
#include "WindowsColorSink.hpp"
namespace jalog::sinks { using DefaultSink = WindowsColorSink; }
#elif defined(__ANDROID__)
#include "AndroidSink.hpp"
namespace jalog::sinks { using DefaultSink = AndroidSink; }
#else
#include "AnsiColorSink.hpp"
namespace jalog::sinks { using DefaultSink = AnsiColorSink; }
#endif
