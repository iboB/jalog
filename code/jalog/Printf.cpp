// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "Printf.hpp"

#include "Scope.hpp"

#include <splat/inline.h>

#include <cstdio>
#include <vector>
#include <cstdarg>

namespace jalog
{

template <uint32_t TrimTrailingNewline>
FORCE_INLINE void addEntryUnchecked(Scope& scope, Level lvl, std::string_view entry)
{
    if constexpr (TrimTrailingNewline) {
        if (!entry.empty() && entry.back() == '\n') {
            entry.remove_suffix(1);
        }
    }
    scope.addEntryUnchecked(lvl, entry);
}

template <uint32_t Flags>
void VPrintf(Scope& scope, Level lvl, const char* format, va_list args)
{
    if constexpr ((Flags & PrintfFlags::SkipLogLevelCheck) == 0) {
        if (!scope.enabled(lvl)) return;
    }

    static constexpr auto TrimTrailingNewline = Flags & PrintfFlags::TrimTrailingNewline;

    va_list acopy;
    // we need to copy the va_list, because if our optimistic fit below doesn't fit, we would need to call
    // vsnprintf again (and the first call would've consumed the original va_list)
    va_copy(acopy, args);

    // optimistically try to fit in a small buffer
    size_t size = 0;
    {
        char buf[1024];
        size = size_t(vsnprintf(buf, sizeof(buf), format, args));

        if (size <= sizeof(buf))
        {
            // success
            addEntryUnchecked<TrimTrailingNewline>(scope, lvl, std::string_view(buf, size));
            return;
        }
    }

    std::vector<char> dbuf(size + 1);
    size = size_t(vsnprintf(dbuf.data(), dbuf.size(), format, acopy));

    addEntryUnchecked<TrimTrailingNewline>(scope, lvl, std::string_view(dbuf.data(), size));
}

template <uint32_t Flags>
void Printf(Scope& scope, Level lvl, const char* format, ...)
{
    if constexpr ((Flags & PrintfFlags::SkipLogLevelCheck) == 0) {
        if (!scope.enabled(lvl)) return;
    }

    va_list args;
    va_start(args, format);
    // force SkipLogLevelCheck as we already checked the level above
    VPrintf<Flags | PrintfFlags::SkipLogLevelCheck>(scope, lvl, format, args);
    va_end(args);
}


template JALOG_API void VPrintf<0>(Scope& scope, Level lvl, const char* format, va_list args);
template JALOG_API void VPrintf<1>(Scope& scope, Level lvl, const char* format, va_list args);
template JALOG_API void VPrintf<2>(Scope& scope, Level lvl, const char* format, va_list args);
template JALOG_API void VPrintf<3>(Scope& scope, Level lvl, const char* format, va_list args);
template JALOG_API void Printf<0>(Scope& scope, Level lvl, const char* format, ...);
template JALOG_API void Printf<1>(Scope& scope, Level lvl, const char* format, ...);
template JALOG_API void Printf<2>(Scope& scope, Level lvl, const char* format, ...);
template JALOG_API void Printf<3>(Scope& scope, Level lvl, const char* format, ...);

}
