cmake_minimum_required(VERSION 3.10)
project(Parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

find_package(FLEX REQUIRED)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

flex_target(Lexer ${SOURCE_DIR}/lexer.l ${SOURCE_DIR}/lexer.cpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
endif()

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/source/*.cpp)

add_executable(parser ${SOURCES} ${FLEX_Lexer_OUTPUTS})

target_include_directories(parser PRIVATE ${INCLUDE_DIR} ${SOURCE_DIR})

target_link_libraries(parser -lfl)

target_link_libraries(parser fl)

option(PRINT_TABLE OFF)
if (PRINT_TABLE STREQUAL "ON")
  add_compile_definitions(${PROJECT_NAME} PRIVATE PRINT_TABLE)
endif()