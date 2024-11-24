cmake_minimum_required(VERSION 3.20)

project(tetrisai)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ROOT_DIR ${CMAKE_SOURCE_DIR}/ai)

set(INCLUDE_DIR ${ROOT_DIR}/include)
set(SRC_DIR ${ROOT_DIR}/src)

set(SOURCES 
    ${SRC_DIR}/State.cpp
    ${SRC_DIR}/Point.cpp
    ${SRC_DIR}/Orientation.cpp
    ${SRC_DIR}/PlayfieldEvaluation.cpp
    ${SRC_DIR}/Queue.cpp
    ${SRC_DIR}/main.cpp
)

add_executable(tetrisai ${SOURCES})

target_include_directories(tetrisai PRIVATE ${INCLUDE_DIR})

target_link_libraries(tetrisai)