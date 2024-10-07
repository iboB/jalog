// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#if defined(_WIN32)
#   include "WindowsColorSink.hpp"
    namespace jalog::sinks { using DefaultSink = WindowsColorSink; }
#elif defined(__ANDROID__)
#   include "AndroidSink.hpp"
    namespace jalog::sinks { using DefaultSink = AndroidSink; }
#elif defined(__APPLE__)
#   include <TargetConditionals.h>
    // only use NSLog as default on iOS
    // on macOS there will be a terminal available and color is prettier
#   if TARGET_OS_IPHONE
#       include "NSLogSink.hpp"
        namespace jalog::sinks { using DefaultSink = NSLogSink; }
#   else
#       include "AnsiColorSink.hpp"
        namespace jalog::sinks { using DefaultSink = AnsiColorSink; }
#   endif
#else
#   include "AnsiColorSink.hpp"
    namespace jalog::sinks { using DefaultSink = AnsiColorSink; }
#endif
