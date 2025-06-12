# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(jalog
    LANGUAGES CXX
)

# cpm
include(./get_cpm.cmake)

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.5.5)

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
CPMAddPackage(gh:iboB/splat@1.3.3)
CPMAddPackage(gh:iboB/itlib@1.11.7)

# charconv
add_library(jalog-charconv INTERFACE)
add_library(jalog::charconv ALIAS jalog-charconv)
include(icm_check_charconv_fp_to_chars)
if(haveCharconvFpToChars)
    message(STATUS "jalog: using std::charconv with fp support")
else()
    CPMAddPackage(gh:iboB/mscharconv@1.2.3)
    message(STATUS "jalog: using msstl/charconv")
    target_link_libraries(jalog-charconv INTERFACE msstl::charconv)
    target_compile_definitions(jalog-charconv INTERFACE JALOG_USE_MSCHARCONV=1)
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
