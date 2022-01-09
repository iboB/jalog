macro(jalog_example example)
    set(ename jalog-example-${example})
    add_executable(${ename} ${ARGN})
    target_link_libraries(${ename} jalog jalog-sinklib ${CMAKE_THREAD_LIBS_INIT})
endmacro()

jalog_example(HelloWorld e-HelloWorld.cpp)
jalog_example(BasicShowcase e-BasicShowcase.cpp)
jalog_example(StreamStyleLogging e-StreamStyleLogging.cpp)

if(NOT JALOG_NO_BUILTIN_ASYNC)
    find_package(Threads REQUIRED)
    jalog_example(AsyncLogging e-AsyncLogging.cpp)
endif()
