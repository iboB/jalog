// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "BenchSink.hpp"
#include <jalog/Entry.hpp>
#include <string_view>

void BenchSink::record(const jalog::Entry& e) {
    size_t d = 0;
    hashAppend(d, e.scope.label());
    hashAppend(d, e.level);
    hashAppend(d, e.text);
    m_digest ^= d;
}
