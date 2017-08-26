include_directories(${CMAKE_SOURCE_DIR}/src)

add_cmocka_test(test_event_queue test_event_queue.c ${CMOCKA_LIBRARY})
