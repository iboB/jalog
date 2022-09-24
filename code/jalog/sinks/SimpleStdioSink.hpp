// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "../Sink.hpp"
#include "../Entry.hpp"

#include <cstdio>

#include <itlib/time_t.hpp>

namespace jalog::sinks
{

class SimpleStdioSink : public Sink
{
public:
    SimpleStdioSink(FILE* out = stdout, FILE* err = nullptr)
        : m_out(out)
        , m_err(err ? err : out)
    {}

    static const char* levelToString(Level l)
    {
        switch (l)
        {
        case Level::Debug:   return "debug";
        case Level::Info:    return "Info ";
        case Level::Warning: return "Warn ";
        case Level::Error:   return "ERROR";
        case Level::Critical:return "CRIT!";
        default: return "  ?  ";
        }
    }

    virtual void record(const Entry& entry) final override {
        FILE* out = entry.level < Level::Error ? m_out : m_err;

        // time
        {
            using namespace std::chrono;
            itlib::time_t tt(system_clock::to_time_t(entry.timestamp));
            auto tm = tt.localtime();
            char buf[32];
            strftime(buf, 32, "%m/%d %X", &tm);

            int ms = int(duration_cast<milliseconds>(entry.timestamp.time_since_epoch()).count() % 1000);

            fprintf(out, "%s.%03d ", buf, ms);
        }

        // scope
        {
            auto lbl = entry.scope.label();
            if (!lbl.empty())
            {
                fprintf(out, "%.*s ", int(lbl.length()), lbl.data());
            }
        }

        // level and text
        fprintf(out, "[%.5s] %.*s\n", levelToString(entry.level), int(entry.text.size()), entry.text.data());
    }

private:
    FILE* m_out;
    FILE* m_err;
};

}
