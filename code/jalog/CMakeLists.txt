# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
icm_add_lib(jalog JALOG)
add_library(jalog::jalog ALIAS jalog)

target_sources(jalog PRIVATE
    API.h

    Level.hpp
    ScopeDesc.hpp
    Entry.hpp

    Scope.hpp
    Scope.cpp
    Logger.hpp
    Logger.cpp

    Sink.hpp
    Sink.cpp

    _qwrite.hpp
    _qwrite.cpp

    BasicStream.hpp

    Printf.hpp
    Printf.cpp

    Log.hpp
    LogPrintf.hpp

    Instance.hpp
    Instance.cpp
)

target_include_directories(jalog INTERFACE ..)
target_link_libraries(jalog
    PUBLIC
        splat::splat
        itlib::itlib
    PRIVATE
        msstl::charconv
)

if(JALOG_NO_BUILTIN_ASYNC)
    target_compile_definitions(jalog PUBLIC -DJALOG_NO_BUILTIN_ASYNC=1)
else()
    target_sources(jalog PRIVATE
        AsyncLogging.hpp
        AsyncLogging.cpp
        AsyncLoggingThread.hpp
    )
    target_link_libraries(jalog PRIVATE xec)
endif()
