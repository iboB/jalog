// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "../Sink.hpp"
#include "../Entry.hpp"
#include "../LevelString.hpp"

#include <iostream>
#include <iomanip>

#include <itlib/time_t.hpp>

namespace jalog::sinks
{

class SimpleStreamSink : public Sink
{
public:
    SimpleStreamSink(std::ostream& out, std::ostream& err)
        : m_out(out)
        , m_err(err)
    {}

    static constexpr std::string_view levelToString(Level l)
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
        auto& out = [&, this]() -> std::ostream& {
            if (entry.level < Level::Error) return m_out;
            return m_err;
        }();

        // time
        {
            using namespace std::chrono;
            itlib::time_t tt(system_clock::to_time_t(entry.timestamp));
            auto tm = tt.localtime();
            out << std::put_time(&tm, "%m/%d %X");

            // milliseconds
            auto ms = duration_cast<milliseconds>(entry.timestamp.time_since_epoch()).count() % 1000;
            const char* padding;
            if (ms < 10) padding = "00";
            else if (ms < 100) padding = "0";
            else padding = "";
            out << "." << padding << ms;
        }

        // scope
        {
            auto lbl = entry.scope.label();
            if (!lbl.empty())
            {
                out << " [" << lbl << "]";
            }
        }

        // level
        out << " [" << levelToString(entry.level) << "] ";

        // text
        out << entry.text << "\n";
    }

private:
    std::ostream& m_out;
    std::ostream& m_err;
};

class SimpleConsoleSink : public SimpleStreamSink
{
public:
    SimpleConsoleSink() : SimpleStreamSink(std::cout, std::cerr) {}
};

}
