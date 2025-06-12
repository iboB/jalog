# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
include(icm_testing)

CPMAddPackage(gh:iboB/doctest-util@0.1.3)

icm_add_test(
    NAME jalog-qwrite
    TARGET test-jalog-qwrite
    LIBRARIES
        doctest::main
        itlib::itlib
        splat::splat
        jalog::charconv
    SOURCES t-qwrite.cpp
)

macro(jalog_test test)
    add_doctest_lib_test(${test} jalog t-${test}.cpp ${ARGN})
endmacro()

jalog_test(core)
jalog_test(defaults)

if(NOT JALOG_NO_BUILTIN_ASYNC)
    find_package(Threads REQUIRED)
    jalog_test(mt LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
endif()
