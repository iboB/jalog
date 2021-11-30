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
        : scope(logger, "t1", 1, 2)
        , scope2(logger, "t2", 3, 4)
        , sink(std::make_shared<TestSink>())
    {
        logger.setup().add(sink);
    }

    jalog::Logger logger;
    jalog::Scope scope;
    jalog::Scope scope2;

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

    void checkChronologicalOrder()
    {
        auto& es = sink->entries;
        CHECK(std::is_sorted(es.begin(), es.end(), [](auto& a, auto& b) {
            return a.timestamp < b.timestamp;
        }));
    }

    TestSink::EntryCopy popFront()
    {
        auto& es = sink->entries;
        if (es.empty()) return {};
        auto ret = std::move(es.front());
        es.erase(es.begin());
        return ret;
    }
};

#define tlog(lvl, ...) JALOG_SCOPE(helper.scope, lvl, __VA_ARGS__)
#define plog(...) \
    tlog(Info, __VA_ARGS__); \
    e = helper.popFront()
#define tlog2(lvl, ...) JALOG_SCOPE(helper.scope2, lvl, __VA_ARGS__)

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

TEST_CASE("levels and scopes")
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

    REQUIRE(es.size() == 7);
    helper.checkChronologicalOrder();

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
}

TEST_CASE("levels")
{
    TestHelper helper;
    auto& es = helper.sink->entries;

    // shouldn't affect already created scopes
    helper.logger.setDefaultLevel(jalog::Level::Off);

    int argCounter = 0;
    auto arg = [&]() { return argCounter++; };

    auto logAll = [&]() {
        tlog(Debug, arg());
        tlog(Info, arg());
        tlog(Warning, arg());
        tlog(Error, arg());
        tlog(Critical, arg());
    };

    logAll();
    CHECK(argCounter == 5);
    CHECK(es.size() == 5);

    argCounter = 0;
    es.clear();
    helper.scope.setLevel(jalog::Level::Error);
    logAll();
    CHECK(argCounter == 2);
    CHECK(es.size() == 2);

    argCounter = 0;
    es.clear();
    helper.scope.setLevel(jalog::Level::Off);
    for (int i=0; i<10; ++i) logAll();
    CHECK(argCounter == 0);
    CHECK(es.empty());

    argCounter = 0;
    es.clear();
    helper.scope.setLevel(jalog::Level::Warning);
    logAll();
    CHECK(argCounter == 3);
    CHECK(es.size() == 3);
}

TEST_CASE("new scopes")
{
    TestHelper helper;
    tlog(Info, "foo");

    {
        helper.logger.setDefaultLevel(jalog::Level::Warning);
        jalog::Scope newScope(helper.logger, "ns1", 10, 20);
        JALOG_SCOPE(newScope, Debug, "dbg1"); // should be skipped
        JALOG_SCOPE(newScope, Info, "info1"); // should be skipped
        JALOG_SCOPE(newScope, Warning, "warn1");
        tlog(Info, "bar");
    }

    {
        helper.logger.setDefaultLevel(jalog::Level::Info);
        jalog::Scope newScope(helper.logger, "ns2", 30, 40);
        JALOG_SCOPE(newScope, Debug, "dbg2"); // should be skipped
        JALOG_SCOPE(newScope, Info, "info2");
        JALOG_SCOPE(newScope, Warning, "warn2");
        tlog(Debug, "baz");
    }

    auto& es = helper.sink->entries;
    CHECK(es.size() == 6);
    helper.checkChronologicalOrder();

    {
        auto e = helper.popFront();
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "foo");
    }
    {
        auto e = helper.popFront();
        CHECK(e.scope.label() == "ns1");
        CHECK(e.scope.id() == 10);
        CHECK(e.scope.userData == 20);
        CHECK(e.level == jalog::Level::Warning);
        CHECK(e.text == "warn1");
    }
    {
        auto e = helper.popFront();
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "bar");
    }
    {
        auto e = helper.popFront();
        CHECK(e.scope.label() == "ns2");
        CHECK(e.scope.id() == 30);
        CHECK(e.scope.userData == 40);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "info2");
    }
    {
        auto e = helper.popFront();
        CHECK(e.scope.label() == "ns2");
        CHECK(e.scope.id() == 30);
        CHECK(e.scope.userData == 40);
        CHECK(e.level == jalog::Level::Warning);
        CHECK(e.text == "warn2");
    }
    {
        auto e = helper.popFront();
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Debug);
        CHECK(e.text == "baz");
    }
}

struct vec { float x, y; };

// have this overload to confitm that the log happens through BasicStream and through std::ostream
std::ostream& operator<<(std::ostream& o, const vec& v)
{
    return o << v.x << ' ' << v.y;
}

jalog::BasicStream& operator,(jalog::BasicStream& s, const vec& v)
{
    return s, '(', v.x, ';', v.y, ')';
}

struct ivec { int x, y; };

std::ostream& operator<<(std::ostream& o, const ivec& v)
{
    return o << v.x << ' ' << v.y;
}

TEST_CASE("text output")
{
    TestHelper helper;
    TestSink::EntryCopy e;

    // bools
    plog("b1 = ", true, ", b2 = ", false);
    CHECK(e.text == "b1 = true, b2 = false");

    // chars
    plog("\t", 'j', 'a', "log");
    CHECK(e.text == "\tjalog");

    // ints
    plog(uint8_t(4), ", ", INT16_MIN, ", ", jalog::base<2>(9), ", ", 1'000'000'000'000ull);
    CHECK(e.text == "4, -32768, 1001, 1000000000000");

    // floats
    plog(3.14159f, ", ", 2.718281828);
    CHECK(e.text == "3.14159, 2.718281828");

    // strings
    std::string str = "abcde";
    std::string_view sv(str.c_str() + 1, 3);
    plog(str, " : ", sv);
    CHECK(e.text == "abcde : bcd");

    // customs
    vec v = {3.00f, 1.12f};
    ivec iv = {5, 6};
    plog("j: ", v, ", std: ", iv);
    CHECK(e.text == "j: (3;1.12), std: 5 6");
}
