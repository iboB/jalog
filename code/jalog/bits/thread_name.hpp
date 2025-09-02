// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <string_view>

namespace jalog::this_thread {
int set_name(std::string_view name) noexcept;
} // namespace jalog
