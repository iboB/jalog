// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "API.h"

namespace jalog
{

class ScopeDesc;
struct Entry;

class JALOG_API Sink
{
protected:
    ~Sink() = default;
public:

    virtual void record(const Entry& entry) = 0;

    // optionally override to flush recorded entries
    virtual void flush();
};

}
