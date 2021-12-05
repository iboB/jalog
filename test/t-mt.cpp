// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <doctest/doctest.h>

#include "TestSink.hpp"

#include <jalog/Instance.hpp>
#include <jalog/Logger.hpp>
#include <jalog/Scope.hpp>
#include <jalog/AsyncLogging.hpp>
#include <jalog/Log.hpp>

#include <thread>

TEST_SUITE_BEGIN("jalog");

#define clog(...) JALOG_SCOPE(common, Info, )

TEST_CASE("mt")
{
    std::vector<jalog::SinkPtr> sinks;
    for (int i=0; i<2; ++i) sinks.push_back(std::make_shared<TestSink>());
    auto tmpSink = std::make_shared<TestSink>();

    {
        jalog::Logger jl;
        jalog::Instance ji(jl);

        {
            auto setup = ji.setup();
            for (auto& s : sinks) setup.async().add(s);
        }

        REQUIRE(!!ji.asyncLogging());

        jalog::Scope common(jl, "common", 1, 2);

        ji.asyncLogging()->add(tmpSink);

        std::thread t1([&]() {
            jalog::Scope st1(jl, "st1", 10, 20);
        });

        std::thread t2([&]() {
            jalog::Scope st2(jl, "st2", 50, 80);
        });

        ji.asyncLogging()->remove(tmpSink);

        t1.join();
        t2.join();
    }
}
