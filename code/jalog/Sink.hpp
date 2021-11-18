// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

namespace jalog
{

class ScopeDesc;
struct Entry;

class JALOG_API Sink
{
public:
    virtual ~Sink();

    virtual void record(const Entry& entry) = 0;
};

}
