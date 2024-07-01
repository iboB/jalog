# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(jalog-sinklib JALOG_SINKLIB)
add_library(jalog::sinklib ALIAS jalog-sinklib)
target_sources(jalog-sinklib PRIVATE
    API.h
    AnsiColorSink.hpp
    AnsiColorSink.cpp
)

if(WIN32)
    target_sources(jalog-sinklib PRIVATE
        WindowsColorSink.hpp
        WindowsColorSink.cpp
    )
endif()

target_include_directories(jalog-sinklib INTERFACE ../..)
target_link_libraries(jalog-sinklib PUBLIC jalog)
