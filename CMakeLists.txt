# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(jalog)

# cpm
include(./get_cpm.cmake)

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.3.4)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${icm_SOURCE_DIR}
)

include(icm_dev_mode)
include(icm_add_lib)

#######################################
# options
option(JALOG_STATIC "jalog: build as static lib" OFF)
option(JALOG_NO_BUILTIN_ASYNC "jalog: disable builtin async logging helpers" OFF)
option(JALOG_BUILD_SINKLIB "jalog: build extension sinklib" ON)
option(JALOG_BUILD_TESTS "jalog: build tests" ${ICM_DEV_MODE})
option(JALOG_BUILD_EXAMPLES "jalog: build examples" ${ICM_DEV_MODE})

#######################################
# packages
CPMAddPackage(gh:iboB/splat@1.2.1)
CPMAddPackage(gh:iboB/itlib@1.6.3)
CPMAddPackage(gh:iboB/mscharconv@1.2.1)

if(NOT JALOG_NO_BUILTIN_ASYNC)
    CPMAddPackage(gh:iboB/xec@1.4.1)
endif()

#######################################
# subdirs

add_subdirectory(code/jalog)

if(JALOG_BUILD_SINKLIB)
    add_subdirectory(ext/jalog/sinks)
endif()

if(ICM_DEV_MODE)
    add_subdirectory(scratch)
endif()

if(JALOG_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(JALOG_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
