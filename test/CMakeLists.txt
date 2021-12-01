include(icm_testing)

CPMAddPackage(gh:iboB/doctest-lib@2.4.6)

icm_add_test(
    NAME jalog-qwrite
    TARGET test-jalog-qwrite
    LIBRARIES
        doctest-main
        itlib
        splat
        mscharconv
    SOURCES t-qwrite.cpp
)

macro(jalog_test test)
    icm_add_doctest_lib_test(${test} jalog ${ARGN})
endmacro()

jalog_test(core t-core.cpp)
jalog_test(defaults t-defaults.cpp)
jalog_test(mt t-mt.cpp)
