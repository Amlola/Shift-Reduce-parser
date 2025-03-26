project(UnitTestTable)

set(TABLE_TEST_SOURCES unit-test-table.cpp)

list(TRANSFORM TABLE_TEST_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

set(TEST_SOURCE_FILE ${CMAKE_SOURCE_DIR}/unit-test-table.cpp)

add_executable(${PROJECT_NAME} ${TABLE_TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE parser-lib gtest gtest_main)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_definitions(${PROJECT_NAME} PRIVATE PRINT_TABLE TEST_FILE_DIR="${TEST_FILE_DIR}/${PROJECT_NAME}/")

set_target_properties(${PROJECT_NAME} PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/expected_tables DESTINATION ${TEST_FILE_DIR}/${PROJECT_NAME})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/received_tables DESTINATION ${TEST_FILE_DIR}/${PROJECT_NAME})