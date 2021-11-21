// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <doctest/doctest.h>

#define I_JALOG_TESTING_QWRITE
#include "../code/jalog/_qwrite.hpp"
#include "../code/jalog/_qwrite.cpp"

#include <msstl/charconv.hpp>
#include <string>
#include <sstream>
#include <limits>

TEST_SUITE_BEGIN("jalog");

template <typename I>
std::string cc2s(I i, int base = 10)
{
    char buf[100];
    auto res = msstl::to_chars(buf, buf+100, i, base);
    return {buf, res.ptr};
}

template <unsigned Base = 10, typename I>
std::string wi2s(I i, int padding = 0, char pad = ' ')
{
    std::stringstream sout;

    jalog::qwrite::write_integer(
        *sout.rdbuf(),
        jalog::qwrite::wrapped_integer<I, Base>{i},
        padding,
        pad
    );

    return sout.str();
}

template <unsigned Base = 10, typename I>
void itest(I i)
{
    CHECK(cc2s(i, int(Base)) == wi2s<Base>(i));
}

template  <typename T>
using lim = std::numeric_limits<T>;

TEST_CASE("wrapped int")
{
    // sanity
    CHECK(cc2s(4373) == "4373");
    CHECK(wi2s(-14322) == "-14322");

    itest(5);
    itest<7>(4521);
    itest<27>(-154521);

    itest<2>(lim<uint8_t>::min());
    itest<2>(lim<uint8_t>::max());
    itest<2>(lim<uint16_t>::min());
    itest<2>(lim<uint16_t>::max());
    itest<2>(lim<uint32_t>::min());
    itest<2>(lim<uint32_t>::max());
    itest<2>(lim<uint64_t>::min());
    itest<2>(lim<uint64_t>::max());
}

TEST_CASE("wrapped padding")
{
    CHECK(wi2s(24, 5, '0') == "00024");
}
