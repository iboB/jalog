// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <doctest/doctest.h>
#include <jalog/jalog.hpp>

TEST_SUITE_BEGIN("jalog");

TEST_CASE("test") {
    CHECK(8 == tpl_add(3, 5));
}
