// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "DefaultLogger.hpp"
#include "Logger.hpp"
#include "DefaultScope.hpp"

#include "sinks/DefaultSink.hpp"

namespace jalog {
namespace {
struct DefaultLoggerT : public Logger {
    DefaultLoggerT() {
        addSink(std::make_shared<sinks::DefaultSink>());
        initialize(InitState::System);
    }
};
} // namespace

Logger& DefaultLogger() {
    static DefaultLoggerT l;
    return l;
}

Scope Default_Scope(std::string_view{});

} // namespace jalog