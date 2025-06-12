// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "BenchSink.hpp"
#include <doctest/doctest.h>

#include <jalog/Logger.hpp>
#include <jalog/Scope.hpp>
#include <jalog/Log.hpp>
#include <jalog/LogPrintf.hpp>

TEST_CASE("bench sink") {
    auto sink = std::make_shared<BenchSink>();
    CHECK(sink->digest() == 0);

    jalog::Logger jl;
    jl.directSetup().add(sink);
    jalog::Scope scope(jl, "test_scope");

    JALOG_SCOPE(scope, Info, "foo");
    JALOG_SCOPE(scope, Debug, "bar");

    const auto d = sink->digest();
    CHECK(d == sink->reset());

    JALOG_SCOPE(scope, Info, "foo");
    JALOG_SCOPE(scope, Debug, "bar");

    CHECK(d == sink->reset());

    JALOG_PRINTF_SCOPE(scope, Info, "foo");
    JALOG_PRINTF_SCOPE(scope, Debug, "bar");

    CHECK(d == sink->reset());

    JALOG_SCOPE(scope, Info, "zoo");
    JALOG_SCOPE(scope, Debug, "bar");

    CHECK(d != sink->reset());

    JALOG_SCOPE(scope, Error, "foo");
    JALOG_SCOPE(scope, Debug, "bar");

    CHECK(d != sink->reset());

    jalog::Scope scope2(jl, "test_scope2");

    JALOG_SCOPE(scope, Info, "foo");
    JALOG_SCOPE(scope2, Debug, "bar");

    CHECK(d != sink->reset());
}
