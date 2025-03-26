cmake_minimum_required(VERSION 3.10)
project(Parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

find_package(FLEX REQUIRED)


set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
flex_target(Lexer ${SOURCE_DIR}/lexer.l ${SOURCE_DIR}/lexer.cpp)

add_library(parser-lib STATIC)
target_include_directories(parser-lib PUBLIC ${INCLUDE_DIR})
target_link_libraries(parser-lib PUBLIC fl)

add_executable(parser)
target_link_libraries(parser PRIVATE parser-lib)
target_compile_features(parser PUBLIC cxx_std_17)

add_subdirectory(source)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
    option(PRINT_TABLE ON)
    option(DUMP_TREE ON)
    add_compile_definitions(${PROJECT_NAME} PRIVATE PRINT_TABLE)
    add_compile_definitions(${PROJECT_NAME} PRIVATE DUMP_TREE)
else()
    option(PRINT_TABLE OFF)
    if (PRINT_TABLE STREQUAL "ON")
        add_compile_definitions(${PROJECT_NAME} PRIVATE PRINT_TABLE)
    endif()
    if (DUMP_TREE STREQUAL "ON")
        add_compile_definitions(${PROJECT_NAME} PRIVATE DUMP_TREE)
    endif()
endif()

set_target_properties(parser parser-lib PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)                     

add_subdirectory(test)