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
elseif(ANDROID)
    target_sources(jalog-sinklib PRIVATE
        AndroidSink.hpp
        AndroidSink.cpp
    )
    target_link_libraries(jalog-sinklib PRIVATE log)
elseif(APPLE)
    target_sources(jalog-sinklib PRIVATE
        NSLogSink.hpp
        NSLogSink.mm
    )
    find_library(COCOA Cocoa)
    if(COCOA)
        # On iOS Cocoa does not exist as a separate framework
        # so just ignore it
        target_link_libraries(jalog-sinklib PRIVATE ${COCOA})
    endif()
endif()

target_include_directories(jalog-sinklib INTERFACE ../..)
target_link_libraries(jalog-sinklib PUBLIC jalog)
