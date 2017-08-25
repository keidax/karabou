cmake_minimum_required(VERSION 3.0)

# Used by YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(karabou C)

add_executable(karabou
    event_queue.c
    karabou.c
    keys.c
    )
target_compile_options(karabou PRIVATE -Wall -Wextra)

install(TARGETS karabou RUNTIME DESTINATION bin)
