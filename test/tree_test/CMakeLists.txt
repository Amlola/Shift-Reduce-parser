project(UnitTestTree)

set(TREE_TEST_SOURCES unit-test-tree.cpp)

list(TRANSFORM TREE_TEST_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

set(TEST_SOURCE_FILE ${CMAKE_SOURCE_DIR}/unit-test-table.cpp)

add_executable(${PROJECT_NAME} ${TREE_TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE parser-lib gtest gtest_main)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_definitions(${PROJECT_NAME} PRIVATE PRINT_TABLE TEST_FILE_DIR="${TEST_FILE_DIR}/${PROJECT_NAME}/")

set_target_properties(${PROJECT_NAME} PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)                    

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/expected_tree DESTINATION ${TEST_FILE_DIR}/${PROJECT_NAME})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/received_tree DESTINATION ${TEST_FILE_DIR}/${PROJECT_NAME})
# file(COPY expected_tree/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/configs)${TEST_FILES_DIR}/${PROJECT_NAME}