set(TEST_FILE_DIR ${CMAKE_BINARY_DIR}/tests/test_files)

file(COPY examples DESTINATION ${TEST_FILE_DIR})

add_subdirectory(table_test)
add_subdirectory(tree_test)