cmake_minimum_required(VERSION 3.0)

# Used by YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(karabou)

add_executable(karabou karabou.c keys.c event_queue.c)
target_compile_options(karabou PRIVATE -Wall -Wextra)

install(TARGETS karabou RUNTIME DESTINATION bin)
