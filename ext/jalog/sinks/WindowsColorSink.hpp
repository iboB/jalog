// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"
#include <jalog/Sink.hpp>

namespace jalog::sinks
{

class JALOG_SINKLIB_API WindowsColorSink final : public Sink
{
public:
    WindowsColorSink();
    virtual void record(const Entry& entry) override;

private:
    void* m_out;
    void* m_err;
};

}
