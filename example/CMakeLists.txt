macro(jalog_example example)
    set(ename jalog-example-${example})
    add_executable(${ename} ${ARGN})
    target_link_libraries(${ename} jalog jalog-sinklib)
endmacro()

jalog_example(HelloWorld e-HelloWorld.cpp)
