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
std::string wi2s(I i, jalog::qwrite::pad padding = {})
{
    std::stringstream sout;

    jalog::qwrite::write_integer(
        *sout.rdbuf(),
        jalog::qwrite::wrapped_integer<I, Base>{i},
        padding
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
    using jalog::qwrite::pad;
    CHECK(wi2s(24, {5, pad::left, '0'}) == "00024");
    CHECK(wi2s(24, {4, pad::inner, ' '}) == "  24");
    CHECK(wi2s(24, {6, pad::right, '='}) == "24====");

    CHECK(wi2s(-1256, {3, pad::left, 'z'}) == "-1256");
    CHECK(wi2s(-1256, {4, pad::inner, 'z'}) == "-1256");
    CHECK(wi2s(-1256, {5, pad::right, 'z'}) == "-1256");

    CHECK(wi2s(-87, {4, pad::left, '0'}) == "0-87");
    CHECK(wi2s(-87, {7, pad::right, 'z'}) == "-87zzzz");
    CHECK(wi2s(-87, {5, pad::inner, 'l'}) == "-ll87");
}

template <typename F>
std::string ccf2s(F f)
{
    char buf[128];
    auto res = msstl::to_chars(buf, buf+sizeof(buf), f);
    return {buf, res.ptr};
}

template <typename F>
std::string wf2s(F f)
{
    std::stringstream sout;
    jalog::qwrite::write_float(*sout.rdbuf(), f);
    return sout.str();
}

template <typename F>
void ftest(F f)
{
    CHECK(ccf2s(f) == wf2s(f));
}

TEST_CASE("float")
{
    // sanity
    CHECK(ccf2s(3.14) == "3.14");
    CHECK(wf2s(-8.2f) == "-8.2");

    ftest(-4.1235);
    ftest(1e-11);
    ftest(4.f);
    ftest(-4.22f);
}
