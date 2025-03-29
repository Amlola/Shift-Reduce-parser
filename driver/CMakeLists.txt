cmake_minimum_required(VERSION 3.10)

project(Driver)

add_executable(${PROJECT_NAME} driver.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE parser-lib)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)