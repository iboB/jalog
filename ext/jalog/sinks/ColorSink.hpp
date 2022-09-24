// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

#if defined(_WIN32)
#include "WindowsColorSink.hpp"
namespace jalog::sinks { using ColorSink = WindowsColorSink; }
#else
#include "AnsiColorSink.hpp"
namespace jalog::sinks { using ColorSink = AnsiColorSink; }
#endif
