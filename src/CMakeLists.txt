add_executable(karabou
    event_queue.c
    karabou.c
    keys.c
    )
target_compile_options(karabou PRIVATE -Wall -Wextra)

install(TARGETS karabou RUNTIME DESTINATION bin)
