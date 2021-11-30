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
#include <jalog/DefaultLogger.hpp>

TEST_SUITE_BEGIN("jalog");

TEST_CASE("default logger/scope")
{
    auto sink = std::make_shared<TestSink>();
    jalog::DefaultLogger().setup()
        .defaultLevel
        add(sink);

    JALOG(Info, "foo",
}
