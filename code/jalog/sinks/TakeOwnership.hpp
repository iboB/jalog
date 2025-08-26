// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

namespace jalog::sinks {
// whether the sink should take ownership of output resources like FILE* or std::ostream*
enum class TakeOwnership { No, Yes };
} // namespace jalog::sinks
