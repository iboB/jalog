// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "NSLogSink.hpp"

#include "../Entry.hpp"

#include <Foundation/Foundation.h>

namespace jalog::sinks
{

namespace
{
const NSString* lstr[] = {
    @"[debug]",
    @"[Info ]",
    @"[WARN ]",
    @"[ERROR]",
    @"[CRIT!]",
};
const NSString* q = @"[  ?  ]";
}

void NSLogSink::record(const Entry& entry)
{
    auto sev = (entry.level < Level::Off) ? lstr[static_cast<uint32_t>(entry.level)] : q;

    auto sv = entry.text;
    auto txt = CFStringCreateWithBytes(nullptr, (const UInt8 *)sv.data(), CFIndex(sv.size()),
        kCFStringEncodingUTF8, false);

    auto lbl = CFStringCreateWithCString(nullptr, entry.scope.labelCStr(), kCFStringEncodingUTF8);
    NSLog(@"%@%@: %@", lbl, sev, txt);

    CFRelease(lbl);
    CFRelease(txt);
}

}
