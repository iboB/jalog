// jalog
// Copyright (c) 2021-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <jalog/Instance.hpp>
#include <jalog/Log.hpp>
#include <jalog/sinks/ColorSink.hpp>

#include <thread>

int main() {
    jalog::Instance jl;
    jl.setup()
        .async()
            .add<jalog::sinks::ColorSink>();

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
