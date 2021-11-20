// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include <itlib/small_vector.hpp>
#include <itlib/mem_streambuf.hpp>

namespace jalog::qwrite
{
#if defined(I_JALOG_TESTING_QWRITE)
// for the qwrite tests use std::streambuf as it makes test code simpler
using streambuf = std::streambuf;
#else
// for the actual library use the final type to avoid virtual callsstd::streambuf
using streambuf = itlib::mem_ostreambuf<itlib::small_vector<char, 1024>>;
#endif

template <typename Integer, unsigned Base = 64>
struct wrapped_integer { Integer i; };

template <typename Integer, unsigned Base>
void write_integer(streambuf& out, const wrapped_integer<Integer, Base> value, int padding = 0, char pad = ' ')
{
    static_assert(Base >= 2 && Base <= 36, "Invalid base");

    char buf[sizeof(Integer) * 8]; // enough for integers in base 2 or greater
    const auto end = buf + sizeof(buf);
    auto p = end; // writing from back to front

    using Unsigned = std::make_unsigned_t<Integer>;
    constexpr auto ubase = Unsigned(Base); // base in same size to avoid promotion warnings

    Unsigned uvalue = Unsigned(value.i);
    if constexpr (std::is_signed_v<Integer>) {
        if (value.i < 0) {
            uvalue = 0 - uvalue;
        }
    }

    static constexpr char char_from_digit[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    do {
        if constexpr(ubase <= 10) {
            // instead of using the lookup dable we just add to '0'
            *--p = char('0' + uvalue % ubase);
        }
        else {
            *--p = char_from_digit[uvalue % ubase];
        }
        uvalue /= ubase;
    } while (uvalue != 0);;

    if constexpr (std::is_signed_v<Integer>) {
        if (value.i < 0) {
            *--p = '-';
        }
    }

    auto len = end - p;
    padding -= len;
    while (padding --> 0) {
        out.sputc(pad);
    }

    out.xsputn(p, len);
}

}
