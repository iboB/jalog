// jalog
// Copyright (c) 2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "API.h"

#include "Level.hpp"
#include "SinkPtr.hpp"

namespace jalog
{

class Logger;
class Sink;

class JALOG_API LoggerSetupDSL
{
public:
    explicit LoggerSetupDSL(Logger& l);
    ~LoggerSetupDSL();

    LoggerSetupDSL& defaultLevel(Level lvl);

    LoggerSetupDSL& addSink(SinkPtr sink);

    template <typename S>
    LoggerSetupDSL& addSink() { return addSink(std::make_shared<S>()); }

private:
    Logger& m_logger;
};

}