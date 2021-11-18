icm_add_lib(jalog-sinklib JALOG_SINKLIB
    API.h
    AnsiColorSink.hpp
    AnsiColorSink.cpp
)

target_include_directories(jalog-sinklib INTERFACE ../..)
target_link_libraries(jalog-sinklib PUBLIC jalog)
