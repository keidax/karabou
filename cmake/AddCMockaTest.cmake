# - ADD_CMOCKA_TEST(test_name test_source linklib1 ... linklibN)

# Copyright (c) 2007      Daniel Gollub <dgollub@suse.de>
# Copyright (c) 2007-2010 Andreas Schneider <asn@cynapses.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

enable_testing()
include(CTest)

# Include the CMocka library
find_package(cmocka REQUIRED)

if(CMAKE_COMPILER_IS_GNUCC AND NOT MINGW)
    set(CMAKE_C_FLAGS_PROFILING "-g -O0 -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wwrite-strings -fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Compiler Flags")
    set(CMAKE_SHARED_LINKER_FLAGS_PROFILING " -fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Linker Flags")
    set(CMAKE_MODULE_LINKER_FLAGS_PROFILING " -fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Linker Flags")
    set(CMAKE_EXEC_LINKER_FLAGS_PROFILING " -fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Linker Flags")
endif(CMAKE_COMPILER_IS_GNUCC AND NOT MINGW)

# Add a custom target to build and run all tests
add_custom_target(mock_test ctest --output-on-failure)

# Assumes existence of test/<test_name>.c
function (ADD_CMOCKA_TEST test_name)
    # Build a binary for this test
    add_executable(${test_name} ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.c)
    # Link against our main internal source, CMocka, and any additional libs
    target_link_libraries(${test_name} ${KARABOU_LIBRARY} ${CMOCKA_LIBRARY} ${ARGN})
    # Tell ctest to use our test binary
    add_test(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name})
    # Make sure we build before testing
    add_dependencies(mock_test ${test_name})
endfunction (ADD_CMOCKA_TEST)
