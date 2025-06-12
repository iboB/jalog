# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
CPMAddPackage(gh:iboB/picobench@2.08)

add_library(jalog-bench-sink STATIC BenchSink.cpp)
set_target_properties(jalog-bench-sink PROPERTIES FOLDER bench)
target_link_libraries(jalog-bench-sink PUBLIC jalog::jalog)

if(JALOG_BUILD_TESTS)
    jalog_test(BenchSink LIBRARIES jalog-bench-sink)
endif()

add_library(jalog-picobench-main STATIC picobench-main.cpp)
set_target_properties(jalog-picobench-main PROPERTIES FOLDER bench)
target_link_libraries(jalog-picobench-main PUBLIC picobench::picobench jalog::jalog)

macro(jalog_benchmark name)
    set(tgt bench-jalog-${name})
    add_executable(${tgt})
    set_target_properties(${tgt} PROPERTIES FOLDER bench)
    target_sources(${tgt} PRIVATE b-${name}.cpp ${ARGN})
    target_link_libraries(${tgt} jalog::jalog jalog-picobench-main jalog-bench-sink)
    add_custom_target(
        run-${tgt}
        COMMAND ${tgt}
    )
    set_target_properties(run-${tgt} PROPERTIES FOLDER bench)
endmacro()

jalog_benchmark(log-types)
