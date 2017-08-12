cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(caps2esc)

add_executable(caps2esc caps2esc.c)
target_compile_options(caps2esc PRIVATE -Wall -Wextra)

add_executable(enter2ctrl enter2ctrl.c)
target_compile_options(enter2ctrl PRIVATE -Wall -Wextra)

install(TARGETS caps2esc RUNTIME DESTINATION bin)
