// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <jalog/Sink.hpp>
#include <cstddef>
#include <utility>

class BenchSink final : public jalog::Sink {
public:
    virtual void record(const jalog::Entry& e) override;

    size_t digest() const {
        return m_digest;
    }
    size_t reset() {
        return std::exchange(m_digest, 0);
    }
private:
    size_t m_digest = 0;
};