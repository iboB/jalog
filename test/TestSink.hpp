// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <jalog/Sink.hpp>
#include <jalog/Entry.hpp>

#include <string>
#include <cstdint>
#include <vector>

class TestSink final : public jalog::Sink
{
public:
    virtual void record(const jalog::Entry& entry) override
    {
        entries.emplace_back(entry);
    }

    struct EntryCopy
    {
        EntryCopy(const jalog::Entry& e)
            : scope(e.scope)
            , level(e.level)
            , timestamp(std::chrono::duration_cast<std::chrono::nanoseconds>(e.timestamp.time_since_epoch()).count())
        {}
        jalog::ScopeDesc scope;
        jalog::Level level;
        uint64_t timestamp;
        std::string text;
    };

    std::vector<EntryCopy> entries;
};
