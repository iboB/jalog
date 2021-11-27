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

TEST_SUITE_BEGIN("jalog");

struct Tester
{
    Tester()
        : testScope(testLogger, "test1", 1, 2)
        , testScope2(testLogger, "test2", 3, 4)
        , sink(std::make_shared<TestSink>())
    {
        testLogger.setup().add(sink);
    }

    jalog::Logger testLogger;
    jalog::Scope testScope;
    jalog::Scope testScope2;

    std::shared_ptr<TestSink> sink;
};

#define tlog(lvl, ...) JALOG_SCOPE(t.testScope, lvl, __VA_ARGS__)
#define tlog2(lvl, ...) JALOG_SCOPE(t.testScope2, lvl, __VA_ARGS__)

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
    Tester t;
    tlog(Debug, "dbg");
    tlog(Info, "info");
    tlog(Warning, "warn1");
    tlog2(Warning, "warn2");
    tlog2(Warning, "warn3");
    tlog2(Error, "err");
    tlog(Critical, "crit");

    auto& e = t.sink->entries;
    CHECK(e.size() == 7);
}
