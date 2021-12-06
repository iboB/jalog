// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"
#include <jalog/Sink.hpp>

namespace jalog::sinks
{

class JALOG_SINKLIB_API AnsiColorSink final : public Sink
{
public:
    virtual void record(const Entry& entry) override;
};

}
