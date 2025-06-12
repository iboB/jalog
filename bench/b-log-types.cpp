// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "BenchSink.hpp"

#include <jalog/Logger.hpp>
#include <jalog/Scope.hpp>

#include <picobench/picobench.hpp>

struct TestInstance {
    jalog::Logger logger;
    std::shared_ptr<BenchSink> sink;
    jalog::Scope sa{logger, "sa"};
    jalog::Scope sb{logger, "sb"};

    TestInstance()
        : sink(std::make_shared<BenchSink>())
    {
        logger.directSetup().add(sink);
    }
};

#include <jalog/LogPrintf.hpp>
void printf(picobench::state& s) {
    TestInstance jl;
    for (auto i : s) {
        JALOG_PRINTF_SCOPE(jl.sa, Info, "Log: %d with %s", i, "str");
    }
    s.set_result(jl.sink->digest());
}
PICOBENCH(printf);

#include <sstream>
void sstream(picobench::state& s) {
    TestInstance jl;
    for (auto i : s) {
        std::stringstream ss;
        ss << "Log: " << i << " with " << "str";
        jl.sa.addEntry(jalog::Level::Info, ss.str());
    }
    s.set_result(jl.sink->digest());
}
PICOBENCH(sstream);

#include <jalog/Log.hpp>
void log(picobench::state& s) {
    TestInstance jl;
    for (auto i : s) {
        JALOG_SCOPE(jl.sa, Info, "Log: ", i, " with ", "str");
    }
    s.set_result(jl.sink->digest());
}
PICOBENCH(log);

#include <jalog/LogStream.hpp>
void stream(picobench::state& s) {
    TestInstance jl;
    for (auto i : s) {
        JALOG_STREAM_SCOPE(jl.sa, Info) << "Log: " << i << " with " << "str";
    }
    s.set_result(jl.sink->digest());
}
PICOBENCH(stream);

#include <jalog/LogFmt.hpp>
void format(picobench::state& s) {
    TestInstance jl;
    for (auto i : s) {
        JALOG_FMT_SCOPE(jl.sa, Info, "Log: {} with {}", i, "str");
    }
    s.set_result(jl.sink->digest());
}
PICOBENCH(format);
