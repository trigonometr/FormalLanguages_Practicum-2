cmake_minimum_required(VERSION 3.9)

project(tests)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/../sources)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/../sources/*.cpp)
add_library(project_lib ${SRC_FILES})

add_subdirectory(gtest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Tests tests.cpp)

target_link_libraries(Tests gtest gtest_main project_lib)

add_test(Tests ${CMAKE_BINARY_DIR}/bin/Tests)
