// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "TakeOwnership.hpp"
#include "../Sink.hpp"
#include "../Entry.hpp"

#include <cstdio>

#include <itlib/time_t.hpp>

namespace jalog::sinks
{

class SimpleStdioSink final : public Sink
{
public:
    SimpleStdioSink(TakeOwnership t, FILE* out, FILE* err = nullptr)
        : m_out(out)
        , m_err(err ? err : out)
        , m_ownsFiles(t == TakeOwnership::Yes)
    {}

    explicit SimpleStdioSink(FILE* out = stdout, FILE* err = nullptr)
        : SimpleStdioSink(TakeOwnership::No, out, err)
    {}

    ~SimpleStdioSink() {
        if (!m_ownsFiles) return;
        if (m_err != m_out) {
            fclose(m_err);
        }
        fclose(m_out);
    }

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

    virtual void record(const Entry& entry) override {
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
    bool m_ownsFiles = false;
};

}
