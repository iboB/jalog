# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(jalog-sinklib JALOG_SINKLIB)
add_library(jalog::sinklib ALIAS jalog-sinklib)
target_sources(jalog-sinklib
    INTERFACE FILE_SET HEADERS FILES
        jalog/sinks/API.h
        jalog/sinks/AnsiColorSink.hpp
        jalog/sinks/DefaultSink.hpp

        # optional sinks, .cpp-s added conditionally below
        jalog/sinks/WindowsColorSink.hpp
        jalog/sinks/AndroidSink.hpp
        jalog/sinks/NSLogSink.hpp
    PRIVATE
        jalog/sinks/AnsiColorSink.cpp
)

if(WIN32)
    target_sources(jalog-sinklib PRIVATE

        jalog/sinks/WindowsColorSink.cpp
    )
elseif(ANDROID)
    target_sources(jalog-sinklib PRIVATE
        jalog/sinks/AndroidSink.cpp
    )
    target_link_libraries(jalog-sinklib PRIVATE log)
elseif(APPLE)
    target_sources(jalog-sinklib PRIVATE
        jalog/sinks/NSLogSink.mm
    )
    find_library(COCOA Cocoa)
    if(COCOA)
        # On iOS Cocoa does not exist as a separate framework
        # so just ignore it
        target_link_libraries(jalog-sinklib PRIVATE ${COCOA})
    endif()
endif()

target_link_libraries(jalog-sinklib PUBLIC jalog)
