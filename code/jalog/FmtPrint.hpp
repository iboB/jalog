// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"
#include "PrintFlags.hpp"
#include "Scope.hpp"
#include "_q_char_alloc.hpp"
#include <itlib/small_vector.hpp>
#include <format>
#include <iterator>

namespace jalog
{

template <uint32_t Flags = PrintFlags::None, typename... Args>
void FmtPrint(Scope& scope, Level lvl, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr ((Flags & PrintFlags::SkipLogLevelCheck) == 0) {
        if (!scope.enabled(lvl)) return;
    }

    itlib::small_vector<char, 1024, 0, qwrite::q_char_alloc> buffer;
    std::format_to(std::back_inserter(buffer), fmt, std::forward<Args>(args)...);
    std::string_view entry(buffer.data(), buffer.size());

    if constexpr (Flags & PrintFlags::TrimTrailingNewline) {
        if (!entry.empty() && entry.back() == '\n') {
            entry.remove_suffix(1);
        }
    }
    scope.addEntryUnchecked(lvl, entry);
}

}
