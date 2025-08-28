// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <jalog/LogStream.hpp>

int main() {
    auto log = JALOG_STREAM(Info);

    log << "Hello";
    log << ", stream world!" << jalog::endl;

    log << jalog::Level::Debug << "jalog::Stream was used";

    return 0;
}
