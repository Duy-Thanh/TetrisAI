cmake_minimum_required(VERSION 3.20)

project(tetrisai)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ROOT_DIR ${CMAKE_SOURCE_DIR}/ai)

set(INCLUDE_DIR ${ROOT_DIR}/include)
set(SRC_DIR ${ROOT_DIR}/src)

add_executable(tetrisai ${SRC_DIR}/main.cpp)

target_include_directories(tetrisai PRIVATE ${INCLUDE_DIR})

target_link_libraries(tetrisai)