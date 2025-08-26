// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"
#include "_q_char_alloc.hpp"

#include <itlib/small_vector.hpp>
#include <itlib/mem_streambuf.hpp>

namespace jalog::qwrite {
#if defined(I_JALOG_TESTING_QWRITE)
// for the qwrite tests use std::streambuf as it makes test code simpler
using streambuf = std::streambuf;
#else
// for the actual library use the final type to avoid virtual calls in std::streambuf
using streambuf = itlib::mem_ostreambuf<itlib::small_vector<char, 1024, 0, q_char_alloc>>;
#endif

struct pad {
    int16_t size = 0;
    enum pad_type : int8_t {
        left,
        right,
        inner
    } type = left;
    char character = ' ';
};

inline void write_padded(streambuf& out, const char* numstr, int length, pad padding) {
    padding.size -= int16_t(length);
    if (padding.size <= 0) {
        out.sputn(numstr, length);
    }
    else if (padding.type == pad::right) {
        out.sputn(numstr, length);
        while (padding.size --> 0) {
            out.sputc(padding.character);
        }
    }
    else {
        if (padding.type == pad::inner && *numstr == '-') {
            out.sputc('-');
            ++numstr;
            --length;
        }
        while (padding.size --> 0) {
            out.sputc(padding.character);
        }
        out.sputn(numstr, length);
    }
}

template <typename Integer, unsigned Base = 10>
struct wrapped_integer { Integer i; };

template <typename Integer, unsigned Base>
void write_integer(streambuf& out, const wrapped_integer<Integer, Base> value, pad padding = {}) {
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

    do {
        if constexpr(ubase <= 10) {
            // instead of using the lookup dable we just add to '0'
            *--p = char('0' + uvalue % ubase);
        }
        else {
            static constexpr char char_from_digit[] = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
            };
            *--p = char_from_digit[uvalue % ubase];
        }
        uvalue /= ubase;
    } while (uvalue != 0);

    int len = int(end - p);

    if constexpr (std::is_signed_v<Integer>) {
        if (value.i < 0) {
            *--p = '-';
            ++len;
        }
    }

    write_padded(out, p, len, padding);
}

enum class float_format {
    general,
    fixed,
    scientific,
    hex,
};

JALOG_API void write_float(streambuf& out, float f, float_format fmt = float_format::general, pad padding = {});
JALOG_API void write_float(streambuf& out, double f, float_format fmt = float_format::general, pad padding = {});
JALOG_API void write_float(streambuf& out, long double f, float_format fmt = float_format::general, pad padding = {});

}
