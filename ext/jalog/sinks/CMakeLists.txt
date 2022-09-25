# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(jalog-sinklib JALOG_SINKLIB
    API.h
    AnsiColorSink.hpp
    AnsiColorSink.cpp
    WindowsColorSink.hpp
    WindowsColorSink.cpp
)
add_library(jalog::sinklib ALIAS jalog-sinklib)
target_include_directories(jalog-sinklib INTERFACE ../..)
target_link_libraries(jalog-sinklib PUBLIC jalog)
