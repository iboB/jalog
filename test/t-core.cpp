// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "TestSink.hpp"

#include <jalog/Log.hpp>
#include <jalog/LogPrintf.hpp>
#include <jalog/LogStream.hpp>
#include <jalog/DefaultLogger.hpp>

#include <jalog/Logger.hpp>
#include <jalog/DefaultScope.hpp>

TEST_SUITE_BEGIN("jalog");

TEST_CASE("scopes")
{
    jalog::Scope s1("s1", 1, 2);

    CHECK(&s1.logger() == &jalog::DefaultLogger());

    auto& s1d = s1.desc();
    CHECK(s1d.id() == 1);
    CHECK(s1d.label() == "s1");
    CHECK(s1d.labelCStr() == s1d.label().data());
    CHECK(strlen(s1d.labelCStr()) == s1d.label().length());
    CHECK(s1d.userData == 2);

    CHECK(s1.level() == jalog::Level::Off);
    s1.setLevel(jalog::Level::Error);
    CHECK(s1.level() == jalog::Level::Error);
    CHECK_FALSE(s1.enabled(jalog::Level::Debug));
    CHECK_FALSE(s1.enabled(jalog::Level::Info));
    CHECK_FALSE(s1.enabled(jalog::Level::Warning));
    CHECK(s1.enabled(jalog::Level::Error));
    CHECK(s1.enabled(jalog::Level::Critical));

    jalog::Scope longname("0123456789ABCDEFG");
    CHECK(longname.desc().label() == "0123456789ABCDE");
}

TEST_CASE("default scope")
{
    auto& s = jalog::Default_Scope;
    CHECK(&s.logger() == &jalog::DefaultLogger());
    auto& sd = s.desc();
    CHECK(sd.id() == 0);
    CHECK(sd.label().empty());
    CHECK(*sd.labelCStr() == 0);
    CHECK(sd.userData == -1);
}

struct TestHelper
{
    TestHelper(int numSinks = 2)
        : scope(logger, "t1", 1, 2)
        , scope2(logger, "t2", 3, 4)
    {
        REQUIRE(numSinks >= 1);
        auto setup = logger.directSetup();
        for (int i = 0; i < numSinks; ++i) {
            setup.add(sinks.emplace_back(std::make_shared<TestSink>()));
        }
    }

    jalog::Logger logger;
    jalog::Scope scope;
    jalog::Scope scope2;

    std::vector<std::shared_ptr<TestSink>> sinks;

    TestSink& sink() {
        return *sinks.front();
    }

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

    void checkSinks()
    {
        TestSink::checkSortedEntries(sink().entries);

        // now check whether all sinks contain the same data
        if (sinks.size() == 1) return;
        for (size_t i = 1; i < sinks.size(); ++i)
        {
            sink().checkSameEntries(*sinks[i]);
        }
    }

    TestSink::EntryCopy popFront()
    {
        auto& es = sink().entries;
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

#define tlogf(lvl, ...) JALOG_PRINTF_SCOPE(helper.scope, lvl, __VA_ARGS__)
#define tlogf2(lvl, ...) JALOG_PRINTF_SCOPE(helper.scope2, lvl, __VA_ARGS__)

#define fmtlog(lvl, ...) JALOG_FMT_SCOPE(helper.scope, lvl, __VA_ARGS__)
#define fmtlog2(lvl, ...) JALOG_FMT_SCOPE(helper.scope, lvl, __VA_ARGS__)

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

TEST_CASE("log scopes")
{
    const uint64_t start = nanotime();

    TestHelper helper(3);
    CHECK(&helper.scope.logger() == &helper.logger);
    CHECK(&helper.scope2.logger() == &helper.logger);

    tlog(Debug, "dbg");
    tlog(Info, "info");
    tlogf(Warning, "warn%d", 1);

    auto& es = helper.sink().entries;
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

    tlog2(Warning, "warn", 2);
    tlogf2(Warning, "warn%d", 3);
    tlog2(Error, "err");
    tlogf(Critical, "crit");

    REQUIRE(es.size() == 7);
    helper.checkSinks();

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
    auto& es = helper.sink().entries;

    // shouldn't affect already created scopes
    helper.logger.setDefaultLevel(jalog::Level::Off);

    int argCounter = 0;
    auto arg = [&]() { return argCounter++; };

    auto logAll = [&]() {
        tlog(Debug, arg());
        tlog(Info, arg());
        tlog(Warning, arg());
        tlogf(Error, "%d", arg());
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

    auto& es = helper.sink().entries;
    CHECK(es.size() == 6);
    helper.checkSinks();

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

TEST_CASE("printf")
{
    TestHelper helper;
    auto& es = helper.sink().entries;

    helper.scope.setLevel(jalog::Level::Info);
    helper.scope2.setLevel(jalog::Level::Info);

    tlogf(Debug, "foo"); // should be skipped
    tlogf(Info, "%d hello %s %.2f", 1, "bb", 3.14159);
    jalog::Printf(helper.scope, jalog::Level::Debug, "%dabc", 12); // should be skipped
    jalog::Printf(helper.scope, jalog::Level::Error, "x%dy\n", 33); // newline is preserved
    jalog::Printf(helper.scope2, jalog::Level::Debug, "%dabc", 12); // should be skipped
    jalog::Printf<jalog::PrintFlags::SkipLogLevelCheck>(helper.scope2, jalog::Level::Debug, "hax");
    jalog::Printf<jalog::PrintFlags::TrimTrailingNewline>(helper.scope2, jalog::Level::Info, "%snl\n", "no");
    jalog::Printf<jalog::PrintFlags::TrimTrailingNewline | jalog::PrintFlags::SkipLogLevelCheck>(
        helper.scope2, jalog::Level::Debug, "no%shax\n", "nl"
    );

    CHECK(es.size() == 5);
    helper.checkSinks();

    {
        auto e = helper.popFront();
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "1 hello bb 3.14");
    }
    {
        auto e = helper.popFront();
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Error);
        CHECK(e.text == "x33y\n");
    }
    {
        auto e = helper.popFront();
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Debug);
        CHECK(e.text == "hax");
    }
    {
        auto e = helper.popFront();
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "nonl");
    }
    {
        auto e = helper.popFront();
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Debug);
        CHECK(e.text == "nonlhax");
    }
}

namespace test {

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

}

#define tstream(lvl) JALOG_STREAM_SCOPE(helper.scope, lvl)

TEST_CASE("stream")
{
    TestHelper helper;
    auto& es = helper.sink().entries;
    helper.scope.setLevel(jalog::Level::Info);
    helper.scope2.setLevel(jalog::Level::Info);

    tstream(Info) << "hello " << test::ivec{1, 2};
    tstream(Debug) << "dbg"; // skipped

    {
        jalog::Stream str(helper.scope2);

        str << "foo " << test::vec{1.2f, 2.1f} << jalog::endl;

        str << jalog::Level::Debug << "dbg2"; // skipped
        str << jalog::Level::Error << "err";
        str << jalog::base<2>(2);
        str << jalog::Level::Info << "info" << 2;
    }

    CHECK(es.size() == 4);
    helper.checkSinks();

    {
        auto e = helper.popFront();
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "hello 1 2");
    }
    {
        auto e = helper.popFront();
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "foo (1.2;2.1)");
    }
    {
        auto e = helper.popFront();
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Error);
        CHECK(e.text == "err10");
    }
    {
        auto e = helper.popFront();
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "info2");
    }
}

TEST_CASE("stream output")
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
    test::vec v = {3.00f, 1.12f};
    test::ivec iv = {5, 6};
    plog("j: ", v, ", std: ", iv);
    CHECK(e.text == "j: (3;1.12), std: 5 6");
}

#include <version>
#if defined(__cpp_lib_format)

#include <jalog/LogFmt.hpp>

TEST_CASE("format")
{
    TestHelper helper;
    auto& es = helper.sink().entries;

    helper.scope.setLevel(jalog::Level::Info);
    helper.scope2.setLevel(jalog::Level::Info);

    fmtlog(Debug, "foo"); // should be skipped
    fmtlog(Info, "{1} hello {0} {2:.2f}", 1, "bb", 3.14159);
    fmtlog(Debug, "{}abc", 12); // should be skipped
    fmtlog(Error, "x{}y\n", 33); // newline is preserved
    fmtlog2(Debug, "{}abc", 12); // should be skipped
    jalog::FmtPrint<jalog::PrintFlags::SkipLogLevelCheck>(helper.scope2, jalog::Level::Debug, "hax");
    jalog::FmtPrint<jalog::PrintFlags::TrimTrailingNewline>(helper.scope2, jalog::Level::Info, "{}nl\n", "no");
    jalog::FmtPrint<jalog::PrintFlags::TrimTrailingNewline | jalog::PrintFlags::SkipLogLevelCheck>(
        helper.scope2, jalog::Level::Debug, "no{}hax\n", "nl"
    );

    CHECK(es.size() == 5);
    helper.checkSinks();

    {
        auto e = helper.popFront();
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "bb hello 1 3.14");
    }
    {
        auto e = helper.popFront();
        helper.checkT1(e);
        CHECK(e.level == jalog::Level::Error);
        CHECK(e.text == "x33y\n");
    }
    {
        auto e = helper.popFront();
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Debug);
        CHECK(e.text == "hax");
    }
    {
        auto e = helper.popFront();
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Info);
        CHECK(e.text == "nonl");
    }
    {
        auto e = helper.popFront();
        helper.checkT2(e);
        CHECK(e.level == jalog::Level::Debug);
        CHECK(e.text == "nonlhax");
    }
}

#endif