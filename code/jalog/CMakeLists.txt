icm_add_lib(jalog JALOG
    API.h
    jalog.hpp
    jalog.cpp
)

target_include_directories(jalog INTERFACE ..)
target_link_libraries(jalog
    PUBLIC
        splat
)
