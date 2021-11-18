icm_add_lib(jalog JALOG
    API.h

    Level.hpp
    ScopeDesc.hpp
    Entry.hpp

    Sink.hpp
    Sink.cpp

    Scope.hpp
    Scope.cpp
    Logger.hpp
    Logger.cpp

    Log.hpp
)

target_include_directories(jalog INTERFACE ..)
target_link_libraries(jalog
    PUBLIC
        splat
        itlib
)
