// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <jalog/Scope.hpp>
#include <jalog/Log.hpp>
#include <jalog/Printf.hpp>
#include <jalog/PrintfWrap.hpp>

///////////////////////////////////////////////////////////////////////////////
// this is an imaginary c library
typedef void(*clib_log_func)(const char* fmt, ...);
clib_log_func clib_log_info;
clib_log_func clib_log_error;
void clib_do_x(int x) {
    if (clib_log_info) clib_log_info("Doing X: %d", x);
    // ...
}
void clib_do_y(double y) {
    if (clib_log_info) clib_log_info("Doing something else: %.3f", y);
    if (clib_log_error) clib_log_error("Uh-oh. An error has occured: %s", "y is bad");
    // ...
}
///////////////////////////////////////////////////////////////////////////////

jalog::Scope clibScope("clib");

void JalogForCLib_Info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    jalog::VPrintf(clibScope, jalog::Level::Info, fmt, args);
    va_end(args);
}

// or alternatively, use the PrintfWrap function
auto JalogForCLib_Error = jalog::PrintfWrap<clibScope, jalog::Level::Error>;

int main() {
    clib_log_info = JalogForCLib_Info;
    clib_log_error = JalogForCLib_Error;

    JALOG(Info, "Launching CLib");
    clib_do_x(43);
    clib_do_y(3.141592);

    return 0;
}
