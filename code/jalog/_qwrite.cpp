// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "_qwrite.hpp"

#include <msstl/charconv.hpp>

#include <cstdio>

namespace jalog::qwrite {

namespace {
template <typename FP>
void write_fp(streambuf& out, FP f, float_format fmt, pad padding) {
    char buf[128]; // hopefully enough

    auto cf = [fmt]() {
        switch (fmt) {
            case float_format::general: return msstl::chars_format::general;
            case float_format::fixed: return msstl::chars_format::fixed;
            case float_format::scientific: return msstl::chars_format::scientific;
            case float_format::hex: return msstl::chars_format::hex;
            default: return msstl::chars_format::general;
        }
    }();

    auto res = msstl::to_chars(buf, buf + sizeof(buf), f, cf);
    if (res.ec != std::errc{}) {
        // it was not enough :(
        // rescue
        res.ptr = buf + snprintf(buf, sizeof(buf), "%f", double(f));
    }

    write_padded(out, buf, int(res.ptr - buf), padding);
}
}

void write_float(streambuf& out, float f, float_format fmt, pad padding) {
    write_fp(out, f, fmt, padding);
}
void write_float(streambuf& out, double f, float_format fmt, pad padding) {
    write_fp(out, f, fmt, padding);
}
void write_float(streambuf& out, long double f, float_format fmt, pad padding) {
    write_fp(out, f, fmt, padding);
}

}
