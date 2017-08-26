project(karabou C)

cmake_minimum_required(VERSION 3.0)

# Used by YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

option(UNIT_TESTING "Whether to build CMocka tests" ON)

if (UNIT_TESTING)
    enable_testing()
    add_subdirectory(test)
endif (UNIT_TESTING)
