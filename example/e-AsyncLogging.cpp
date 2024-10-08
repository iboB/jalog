// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <jalog/Instance.hpp>
#include <jalog/Log.hpp>
#include <jalog/sinks/DefaultSink.hpp>

#include <thread>

int main() {
    jalog::Instance jl;
    jl.setup()
        .async()
            .add<jalog::sinks::DefaultSink>();

    std::thread threadA([]() {
        for (int i=0; i<10; ++i) {
            JALOG(Info, "Doing thing ", i, " in thread A");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    std::thread threadB([]() {
        for (int i=0; i<18; ++i) {
            JALOG(Debug, "Doing thing ", i, " (more often) in thread B");
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    });

    threadA.join();
    threadB.join();

    return 0;
}
