// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "BenchSink.hpp"
#include <jalog/Entry.hpp>
#include <string_view>

namespace {
template <typename T>
void hashAppend(std::size_t& digest, T t) {
    digest ^= std::hash<T>{}(t)+0x9e3779b9 + (digest << 6) + (digest >> 2);
}
}

void BenchSink::record(const jalog::Entry& e) {
    hashAppend(m_digest, e.scope.label());
    hashAppend(m_digest, e.level);
    hashAppend(m_digest, e.text);
}
