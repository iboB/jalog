// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <iostream>

#include "BenchSink.hpp"

#include <jalog/Instance.hpp>
#include <jalog/Logger.hpp>
#include <jalog/Scope.hpp>
#include <jalog/Log.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/async_logger.h>
#include <spdlog/async.h>
#include <spdlog/sinks/base_sink.h>

#include <picobench/picobench.hpp>

#include <thread>
#include <mutex>

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

template <typename Mutex>
class SpdlogHashingSink : public spdlog::sinks::base_sink<Mutex> {
    size_t m_digest = 0;

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
public:
    size_t digest() const {
        return m_digest;
    }
    size_t reset() {
        return std::exchange(m_digest, 0);
    }
};

PICOBENCH_SUITE("single thread");

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
    auto sink = std::make_shared<SpdlogHashingSink<spdlog::details::null_mutex>>();
    spdlog::logger spdl("hi", {sink});

    for (auto i : s) {
        spdl.info("Log: {} with {}", i, "str");
    }
    s.set_result(sink->digest());
}
PICOBENCH(spdlog_log);

PICOBENCH_SUITE("multi thread");

constexpr size_t Num_Threads = 4;

void jalog_mt(picobench::state& s) {
    jalog::Logger jl;
    auto sink = std::make_shared<BenchSink>();
    jalog::Scope scope{jl, "mt"};
    jalog::Instance inst(jl);
    inst.setup().async().add(sink);

    const int num_msg = s.iterations();

    std::vector<std::thread> threads;
    threads.reserve(Num_Threads);

    {
        picobench::scope bench(s);

        for (size_t i = 0; i < Num_Threads; ++i) {
            threads.emplace_back([i, &scope, &num_msg]() {
                for (int imsg = 0; imsg < num_msg; ++imsg) {
                    JALOG_SCOPE(scope, Info, "Thread ", i, " msg ", imsg);
                }
            });
        }
        for (auto& t : threads) t.join();
    }

    jl.flush();
    s.set_result(sink->digest());
}
PICOBENCH(jalog_mt);

void spdlog_mt(picobench::state& s) {
    //auto sink = std::make_shared<SpdlogHashingSink<std::mutex>>();
    auto sink = std::make_shared<SpdlogHashingSink<spdlog::details::null_mutex>>();
    spdlog::init_thread_pool(Num_Threads * s.iterations(), 1);
    auto logger = std::make_shared<spdlog::async_logger>("mt", sink, spdlog::thread_pool());

    const int num_msg = s.iterations();

    std::vector<std::thread> threads;
    threads.reserve(Num_Threads);

    {
        picobench::scope bench(s);
        for (size_t i = 0; i < Num_Threads; ++i) {
            threads.emplace_back([i, &logger, &num_msg]() {
                for (int imsg = 0; imsg < num_msg; ++imsg) {
                    logger->info("Thread {} msg {}", i, imsg);
                }
            });
        }
        for (auto& t : threads) t.join();
    }

    logger->flush();
    spdlog::shutdown();
    s.set_result(sink->digest());
}
PICOBENCH(spdlog_mt);