# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(jalog JALOG)
add_library(jalog::jalog ALIAS jalog)

target_sources(jalog
    INTERFACE FILE_SET HEADERS FILES
        jalog/API.h

        jalog/Level.hpp
        jalog/ScopeDesc.hpp
        jalog/Entry.hpp

        jalog/Scope.hpp
        jalog/Logger.hpp

        jalog/Sink.hpp
        jalog/sinks/SimpleOStreamSink.hpp
        jalog/sinks/SimpleStdioSink.hpp

        jalog/_qwrite.hpp

        jalog/BasicStream.hpp

        jalog/Printf.hpp
        jalog/PrintfWrap.hpp

        jalog/Log.hpp
        jalog/LogPrintf.hpp
        jalog/LogStream.hpp

        jalog/Instance.hpp
    PRIVATE
        jalog/Scope.cpp
        jalog/Logger.cpp
        jalog/Sink.cpp
        jalog/_qwrite.cpp
        jalog/Printf.cpp
        jalog/Instance.cpp
)

target_link_libraries(jalog
    PUBLIC
        splat::splat
        itlib::itlib
    PRIVATE
        jalog::charconv
)

if(JALOG_NO_BUILTIN_ASYNC)
    target_compile_definitions(jalog PUBLIC -DJALOG_NO_BUILTIN_ASYNC=1)
else()
    target_sources(jalog
        INTERFACE FILE_SET HEADERS FILES
            jalog/AsyncLogging.hpp
            jalog/AsyncLoggingThread.hpp
        PRIVATE
            jalog/AsyncLogging.cpp
    )
endif()
