# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#

if(NOT JALOG_NO_BUILTIN_ASYNC)
    set(asyncSources
        AsyncLogging.hpp
        AsyncLogging.cpp
        AsyncLoggingThread.hpp
    )
endif()

icm_add_lib(jalog JALOG
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

    ${asyncSources}
)

target_include_directories(jalog INTERFACE ..)
target_link_libraries(jalog
    PUBLIC
        splat
        itlib
    PRIVATE
        mscharconv
)

if(JALOG_NO_BUILTIN_ASYNC)
    target_compile_definitions(jalog PUBLIC -DJALOG_NO_BUILTIN_ASYNC=1)
else()
    target_link_libraries(jalog PRIVATE xec)
endif()
