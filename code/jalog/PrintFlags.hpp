// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <cstdint>

namespace jalog
{

namespace PrintFlags {
enum Flags : uint32_t {
    None = 0,

    // skip checking the log level of the scope
    // useful when this check is already done before calling Printf
    SkipLogLevelCheck = 1,

    // trim the trailing newline from the message
    // useful when replacing existing log messages (e.g. with regular printf) which already have a newline at the end
    TrimTrailingNewline = 2,
};
}

}
