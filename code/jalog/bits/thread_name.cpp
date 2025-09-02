// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "thread_name.hpp"

#if defined(_WIN32) && !defined(_POSIX_THREADS)
#   define WIN32_THREADS 1
#else
#   define WIN32_THREADS 0
#endif

#if WIN32_THREADS
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   include <string>
using tid = HANDLE;
#else
#   include <cstring>
#   include <cstdio>
#   if defined(__ANDROID__)
#       include <sys/prctl.h>
#   endif
using tid = pthread_t;
#endif

#include <cassert>

namespace jalog::this_thread {

namespace {
tid get_native_handle() {
#if WIN32_THREADS
    return GetCurrentThread();
#else
    return pthread_self();
#endif
}
}

int set_name(std::string_view name) {
    // max length for thread names with pthread is 16 symbols
    // even though we don't have the same limitation on Windows, we assert anyway for multiplatform-ness
    // note that the length is safe-guarded below
    // you can remove this assertion and the thread names will be trimmed on unix
    // however it's here so we have a notification if we violate the rule
    assert(name.length() < 16);

    tid h = get_native_handle();

#if WIN32_THREADS
    std::wstring ww;
    ww.reserve(name.length());
    for (auto c : name) {
        ww.push_back(wchar_t(c));
    }
    auto res = SetThreadDescription(h, ww.c_str());
    // hresult hax
    if (SUCCEEDED(res)) return 0;
    return res;
#else
    char name16[16];
    auto len = std::min(name.length() + 1, size_t(16));
    snprintf(name16, len, "%s", name.data());
    return pthread_setname_np(h, name16);
#endif
}
} // namespace jalog
