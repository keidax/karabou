add_executable(test_karabou
    test_event_queue.c)

add_test(run_test_karabou test_karabou)

# Cmocka setup
find_package(cmocka REQUIRED)
target_link_libraries(test_karabou ${CMOCKA_LIBRARY})

# For `make test` to build tests first
add_test(build_test_karabou ${CMAKE_COMMAND}
    --build ${CMAKE_CURRENT_BINARY_DIR}
    --target test_karabou)
set_tests_properties(run_test_karabou PROPERTIES DEPENDS build_test_karabou)
