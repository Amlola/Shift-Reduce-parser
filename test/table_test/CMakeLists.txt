cmake_minimum_required(VERSION 3.10)

project(UnitTestTable)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../../include)

set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/../../source/parser.cpp 
    ${CMAKE_SOURCE_DIR}/../../source/utils.cpp
    ${CMAKE_SOURCE_DIR}/../../source/lexer_helper.cpp
)

find_package(FLEX REQUIRED)
flex_target(Lexer ${CMAKE_SOURCE_DIR}/../../source/lexer.l ${CMAKE_BINARY_DIR}/lexer.cpp)

set(TEST_SOURCE_FILE ${CMAKE_SOURCE_DIR}/unit-test-table.cpp)

add_executable(${PROJECT_NAME}
    ${TEST_SOURCE_FILE}
    ${SOURCE_FILES}
    ${FLEX_Lexer_OUTPUTS} 
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} gtest)

target_link_libraries(${PROJECT_NAME} -lfl)

target_link_libraries(${PROJECT_NAME} fl)

add_compile_definitions(${PROJECT_NAME} PRIVATE PRINT_TABLE)