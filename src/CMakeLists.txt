# We'll use this to link against the main executable and also test binaries
set(KARABOU_LIBRARY __karabou CACHE STRING "Internal Karabou library")

# Add new source files here
set(lib_files
    event_queue.c
    keys.c)

add_library(${KARABOU_LIBRARY} STATIC ${lib_files})

add_executable(karabou karabou.c)
target_link_libraries(karabou ${KARABOU_LIBRARY})
