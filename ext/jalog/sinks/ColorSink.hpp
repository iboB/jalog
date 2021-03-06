// jalog
// Copyright (c) 2021-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#if defined(_WIN32)
#include "WindowsColorSink.hpp"
namespace jalog::sinks { using ColorSink = WindowsColorSink; }
#else
#include "AnsiColorSink.hpp"
namespace jalog::sinks { using ColorSink = AnsiColorSink; }
#endif
