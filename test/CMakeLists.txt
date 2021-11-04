include(icm_testing)

CPMAddPackage("gh:iboB/doctest-lib@2.4.6")

macro(jalog_test test)
    icm_add_doctest_lib_test(${test} jalog ${ARGN})
endmacro()

jalog_test(core t-core.cpp)
