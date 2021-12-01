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
#include <jalog/LogPrintf.hpp>
#include <jalog/Logger.hpp>
#include <jalog/DefaultLogger.hpp>

TEST_SUITE_BEGIN("jalog");

jalog::Scope gscope("g", 10, 20);

TEST_CASE("default logger/scope")
{
    auto sink = std::make_shared<TestSink>();
    auto& es = sink->entries;
    jalog::DefaultLogger().setup()
        .defaultLevel(jalog::Level::Info)
        .add(sink);

    JALOG(Debug, "dbg", 1);
    JALOG_PRINTF(Info, "info%d", 1);
    JALOG(Error, "error", 1);

    jalog::Scope scope("s1", 1, 2);
    JALOG_SCOPE(scope, Debug, "dbg", 2);
    JALOG_SCOPE(scope, Info, "info", 2);
    JALOG_PRINTF_SCOPE(scope, Critical, "crit%d", 2);

    JALOG_PRINTF_SCOPE(gscope, Debug, "dbg%d", 3);
    JALOG_SCOPE(gscope, Error, "err", 3);

    REQUIRE(es.size() == 5);

    {
        auto& e = es[0];
        CHECK(e.scope.label().empty());
        CHECK(e.scope.id() == 0);
        CHECK(e.scope.userData == -1);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "info1");
    }
    {
        auto& e = es[1];
        CHECK(e.scope.label().empty());
        CHECK(e.scope.id() == 0);
        CHECK(e.scope.userData == -1);
        CHECK(e.level == jalog::Level::Error);
        CHECK(e.text == "error1");
    }
    {
        auto& e = es[2];
        CHECK(e.scope.label() == "s1");
        CHECK(e.scope.id() == 1);
        CHECK(e.scope.userData == 2);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "info2");
    }
    {
        auto& e = es[3];
        CHECK(e.scope.label() == "s1");
        CHECK(e.scope.id() == 1);
        CHECK(e.scope.userData == 2);
        CHECK(e.level == jalog::Level::Critical);
        CHECK(e.text == "crit2");
    }
    {
        auto& e = es[4];
        CHECK(e.scope.label() == "g");
        CHECK(e.scope.id() == 10);
        CHECK(e.scope.userData == 20);
        CHECK(e.level == jalog::Level::Error);
        CHECK(e.text == "err3");
    }
}
