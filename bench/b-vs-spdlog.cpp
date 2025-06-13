// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <iostream>

#include "BenchSink.hpp"

#include <jalog/Logger.hpp>
#include <jalog/Scope.hpp>
#include <jalog/Log.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/base_sink.h>

#include <picobench/picobench.hpp>

constexpr jalog::Level lcvt(spdlog::level::level_enum spdl) {
    using namespace spdlog::level;
    using jalog::Level;
    switch (spdl) {
    case trace: [[fallthrough]];
    case debug: return Level::Debug;
    case info: return Level::Info;
    case warn: return Level::Warning;
    case err: return Level::Error;
    case critical: return Level::Critical;
    default:
        return Level::Off;
    }
}

class SpdlogHashingSink : public spdlog::sinks::base_sink<spdlog::details::null_mutex> {
    size_t m_digest = 0;
public:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        size_t d = 0;
        BenchSink::hashAppend(d,
            std::string_view(msg.logger_name.begin(), msg.logger_name.end())
        );
        BenchSink::hashAppend(d, lcvt(msg.level));
        BenchSink::hashAppend(d,
            std::string_view(msg.payload.begin(), msg.payload.end())
        );
        m_digest ^= d;
    }
    void flush_() override {}

    size_t digest() const {
        return m_digest;
    }
    size_t reset() {
        return std::exchange(m_digest, 0);
    }
};

#include <jalog/LogPrintf.hpp>
void jalog_log(picobench::state& s) {
    jalog::Logger jl;
    auto sink = std::make_shared<BenchSink>();
    jalog::Scope scope{jl, "hi"};
    jl.directSetup().add(sink);

    for (auto i : s) {
        JALOG_SCOPE(scope, Info, "Log: ", i, " with ", "str");
    }
    s.set_result(sink->digest());
}
PICOBENCH(jalog_log);

void spdlog_log(picobench::state& s) {
    auto sink = std::make_shared<SpdlogHashingSink>();
    spdlog::logger spdl("hi", {sink});

    for (auto i : s) {
        spdl.info("Log: {} with {}", i, "str");
    }
    s.set_result(sink->digest());
}
PICOBENCH(spdlog_log);
