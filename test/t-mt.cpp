// jalog
// Copyright (c) 2021-2022 Borislav Stanimirov
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

#include <itlib/qalgorithm.hpp>

#include <thread>

TEST_SUITE_BEGIN("jalog");

#define clog(...) JALOG_SCOPE(common, Info, __VA_ARGS__)
#define tlog(...) JALOG_SCOPE(st, Info, __VA_ARGS__)
#define tlog(...) JALOG_SCOPE(st, Info, __VA_ARGS__)

const TestSink::EntryCopy* findByText(const std::vector<TestSink::EntryCopy>& es, std::string_view text) {
    return itlib::pfind_if(es, [text](const TestSink::EntryCopy& e) { return e.text == text; });
}

TEST_CASE("mt")
{
    std::vector<std::shared_ptr<TestSink>> sinks;
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

        clog("before tmp");

        ji.asyncLogging()->add(tmpSink);

        clog("starting t", 1);
        std::thread t1([&]() {
            clog("in t", 1);
            jalog::Scope st(jl, "st1", 10, 20);
            tlog(1);
            tlog(2);
            clog("mix", 1);
            tlog(3);
        });

        clog("starting t", 2);
        std::thread t2([&]() {
            clog("in t", 2);
            jalog::Scope st(jl, "st2", 50, 80);
            tlog(5);
            tlog(6);
            clog("mix", 2);
            tlog(7);
            tlog(8);
        });

        clog("last tmp");
        ji.asyncLogging()->remove(tmpSink);
        clog("after tmp");

        t1.join();
        t2.join();
        clog("joined");
    }

    auto& tmps = tmpSink->entries;
    CHECK(tmps.size() >= 3);
    CHECK(tmps.size() <= 14);

    sinks.front()->checkSameEntries(*sinks.back());

    auto& es = sinks.front()->entries;
    REQUIRE(es.size() == 17);

    std::vector<TestSink::EntryCopy> cs, t1s, t2s;
    for(const auto& e : es)
    {
        CHECK(e.level == jalog::Level::Info);
        if (e.scope.userData == 2)
        {
            CHECK(e.scope.label() == "common");
            CHECK(e.scope.id() == 1);
            cs.push_back(e);
        }
        else if (e.scope.userData == 20)
        {
            CHECK(e.scope.label() == "st1");
            CHECK(e.scope.id() == 10);
            t1s.push_back(e);
        }
        else
        {
            CHECK(e.scope.label() == "st2");
            CHECK(e.scope.id() == 50);
            CHECK(e.scope.userData == 80);
            t2s.push_back(e);
        }
    }

    REQUIRE(t1s.size() == 3);
    TestSink::checkSortedEntries(t1s);
    CHECK(t1s[0].text == "1");
    CHECK(t1s[1].text == "2");
    CHECK(t1s[2].text == "3");
    REQUIRE(t2s.size() == 4);
    CHECK(t2s[0].text == "5");
    CHECK(t2s[1].text == "6");
    CHECK(t2s[2].text == "7");
    CHECK(t2s[3].text == "8");
    TestSink::checkSortedEntries(t2s);
    REQUIRE(cs.size() == 10);
    CHECK(cs[0].text == "before tmp");
    CHECK(cs[1].text == "starting t1");
    CHECK(cs.back().text == "joined");

    // check relative oreder from here as there's no guarantee
    // for a specific one after cs[1];
    auto st1 = &cs[1];

    auto int1 = findByText(cs, "in t1");
    auto mix1 = findByText(cs, "mix1");
    CHECK(int1 > st1);
    CHECK(mix1 > int1);

    auto st2 =  findByText(cs, "starting t2");
    auto int2 = findByText(cs, "in t2");
    auto mix2 = findByText(cs, "mix2");
    CHECK(st2 > st1);
    CHECK(int2 > st2);
    CHECK(mix2 > int2);

    auto ltmp = findByText(cs, "last tmp");
    auto atmp = findByText(cs, "after tmp");
    CHECK(ltmp > st1);
    CHECK(atmp > ltmp);
}
