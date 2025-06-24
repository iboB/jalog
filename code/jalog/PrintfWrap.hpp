// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "Printf.hpp"

// Wrapper function for printf-style logging
//
// It can be used for C libraries (or retro C++ ones) which have can be
// configured with a function pointer of this type.
//
// For example:
// struct CLibConfig {
//     void (*logInfo)(const char*, ...);
//     void (*logError)(const char*, ...);
//     // ...
// };
// ...
// jalog::Scope myscope("my_c_lib_scope");
// ...
// CLibConfig cfg = {};
// cfg.logInfo = jalog::PrintfWrap<myscope, jalog::Level::Info>;
// cfg.logError = jalog::PrintfWrap<myscope, jalog::Level::Error>;
// ...
// CLibInit(&cfg);

namespace jalog {

#if defined(__GNUC__)
#   define I_JALOG_PRINTF_WRAP_FMT __attribute__((format(printf, 1, 2)))
#else
#   define I_JALOG_PRINTF_WRAP_FMT
#endif

template <Scope& scope, Level lvl>
I_JALOG_PRINTF_WRAP_FMT void PrintfWrap(_Printf_format_string_ const char* format, ...) {
    va_list args;
    va_start(args, format);
    ::jalog::VPrintf<PrintFlags::TrimTrailingNewline>(scope, lvl, format, args);
    va_end(args);
}

} // namespace jalog
