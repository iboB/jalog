// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <doctest/doctest.h>

#include "TestSink.hpp"

#include <jalog/Log.hpp>

#include <jalog/Logger.hpp>

#include <algorithm>

TEST_SUITE_BEGIN("jalog");

struct TestHelper
{
    TestHelper()
        : testScope(testLogger, "t1", 1, 2)
        , testScope2(testLogger, "t2", 3, 4)
        , sink(std::make_shared<TestSink>())
    {
        testLogger.setup().add(sink);
    }

    jalog::Logger testLogger;
    jalog::Scope testScope;
    jalog::Scope testScope2;

    std::shared_ptr<TestSink> sink;

    static void checkT1(const TestSink::EntryCopy& e)
    {
        CHECK(e.scope.label() == "t1");
        CHECK(e.scope.id() == 1);
        CHECK(e.scope.userData == 2);
    }

    static void checkT2(const TestSink::EntryCopy& e)
    {
        CHECK(e.scope.label() == "t2");
        CHECK(e.scope.id() == 3);
        CHECK(e.scope.userData == 4);
    }
};

#define tlog(lvl, ...) JALOG_SCOPE(helper.testScope, lvl, __VA_ARGS__)
#define tlog2(lvl, ...) JALOG_SCOPE(helper.testScope2, lvl, __VA_ARGS__)

// logging to an unsetupped logger is safe and does nothing
TEST_CASE("no setup")
{
    jalog::Logger uLogger;
    jalog::Scope uScope(uLogger, "u", 1, 2);

    JALOG_SCOPE(uScope, Info, "asdf");

    int nope = 0;
    auto f = [&nope]() {
        ++nope;
        return 64;
    };
    JALOG_SCOPE(uScope, Critical, f());

    CHECK(nope == 0);
}

TEST_CASE("levels")
{
    const uint64_t start = nanotime();

    TestHelper helper;
    tlog(Debug, "dbg");
    tlog(Info, "info");
    tlog(Warning, "warn1");

    auto& es = helper.sink->entries;
    REQUIRE(es.size() == 3);

    {
        auto& e = es[0];
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Debug);
        CHECK(e.text == "dbg");
        CHECK(e.timestamp >= start);
    }
    {
        auto& e = es[1];
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "info");
    }
    {
        auto& e = es[2];
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Warning);
        CHECK(e.text == "warn1");
    }

    tlog2(Warning, "warn2");
    tlog2(Warning, "warn3");
    tlog2(Error, "err");
    tlog(Critical, "crit");

    {
        auto& e = es[3];
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Warning);
        CHECK(e.text == "warn2");
    }
    {
        auto& e = es[4];
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Warning);
        CHECK(e.text == "warn3");
    }
    {
        auto& e = es[5];
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Error);
        CHECK(e.text == "err");
    }
    {
        auto& e = es[6];
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Critical);
        CHECK(e.text == "crit");
    }

    REQUIRE(es.size() == 7);

    CHECK(std::is_sorted(es.begin(), es.end(), [](auto& a, auto& b) { return a.timestamp < b.timestamp; }));
}
