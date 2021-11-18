icm_add_lib(jalog JALOG
    API.h

    Level.hpp
    LevelString.hpp
    ScopeDesc.hpp
    Entry.hpp

    Scope.hpp
    Scope.cpp
    Logger.hpp
    Logger.cpp

    Log.hpp

    Sink.hpp
    Sink.cpp

    Printf.hpp
    Printf.cpp
)

target_include_directories(jalog INTERFACE ..)
target_link_libraries(jalog
    PUBLIC
        splat
        itlib
)
