// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "AndroidSink.hpp"

#include "../Entry.hpp"

#include <android/log.h>

namespace jalog::sinks {

namespace {
android_LogPriority jalogToAndroid(jalog::Level level) {
    switch (level) {
    case jalog::Level::Debug: return ANDROID_LOG_DEBUG;
    case jalog::Level::Info: return ANDROID_LOG_INFO;
    case jalog::Level::Warning: return ANDROID_LOG_WARN;
    case jalog::Level::Error: return ANDROID_LOG_ERROR;
    case jalog::Level::Critical: return ANDROID_LOG_FATAL;
    default: return ANDROID_LOG_DEFAULT;
    }
}
}

void AndroidSink::record(const Entry& entry) {
    auto lbl = entry.scope.labelCStr();
    auto txt = entry.text;

    __android_log_print(jalogToAndroid(entry.level), lbl, "%.*s", int(txt.length()), txt.data());
}

}
