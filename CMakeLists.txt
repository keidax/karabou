project(karabou C)

cmake_minimum_required(VERSION 3.0)

# Used by YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "-Wall -Wextra")

add_subdirectory(src)

option(UNIT_TESTING "Whether to build CMocka tests" ON)
if (UNIT_TESTING)
    add_definitions(-DUNIT_TESTING)

    # Include our cmocka module
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
    include(AddCMockaTest)

    add_subdirectory(test)
endif (UNIT_TESTING)
