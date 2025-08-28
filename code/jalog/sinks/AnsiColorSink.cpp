// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "AnsiColorSink.hpp"

#include "../Entry.hpp"

#include <itlib/time_t.hpp>

#include <cstdio>

namespace jalog::sinks
{

void AnsiColorSink::record(const Entry& entry)
{
    FILE* out = entry.level < Level::Error ? stdout : stderr;

    // time
    {
        using namespace std::chrono;
        itlib::time_t tt(system_clock::to_time_t(entry.timestamp));
        auto tm = tt.localtime();

        char buf[32];
        strftime(buf, 32, "%m/%d %X", &tm);

        int ms = int(duration_cast<milliseconds>(entry.timestamp.time_since_epoch()).count() % 1000);

        fprintf(out, "\033[34m%s.%03d\033[m ", buf, ms);
    }

    // scope
    {
        auto lbl = entry.scope.label();
        if (!lbl.empty())
        {
            fprintf(out, "\033[36m%.*s\033[m ", int(lbl.length()), lbl.data());
        }
    }

    // level
    {
        static const char* const lstr[] = {
            "\033[35m[debug]\033[m",
            "\033[01;36m[Info ]\033[m",
            "\033[01;33m[WARN ]\033[m",
            "\033[31m[ERROR]\033[m",
            "\033[01;31m[CRIT!]\033[m",
        };
        static_assert(std::size(lstr) == static_cast<uint32_t>(Level::Off));
        const char* str = (entry.level < Level::Off) ? lstr[static_cast<uint32_t>(entry.level)] : "[  ?  ]";
        fputs(str, out);
    }

    // text
    fprintf(out, " %.*s\n", int(entry.text.size()), entry.text.data());
}

}
