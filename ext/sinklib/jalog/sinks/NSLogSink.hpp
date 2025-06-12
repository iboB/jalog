// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"
#include <jalog/Sink.hpp>

namespace jalog::sinks
{

class JALOG_SINKLIB_API NSLogSink final : public Sink
{
public:
    virtual void record(const Entry& entry) override;
};

}
