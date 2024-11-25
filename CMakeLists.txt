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
    ${SRC_DIR}/Tetriminos.cpp
    ${SRC_DIR}/Searcher.cpp
    ${SRC_DIR}/main.cpp
)

# First create the target
add_executable(${PROJECT_NAME} ${SOURCES})

# Then add include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Then add compile and link options
if(MSVC)
    # Enable multi-processor compilation
    target_compile_options(${PROJECT_NAME} PRIVATE 
        /MP             # Enable multi-processor compilation
        /Zc:__cplusplus # Enable proper __cplusplus macro
        /permissive-   # Standards conformance
        /Zc:preprocessor # Enable standards-conforming preprocessor
        /EHsc          # Exception handling model
        /sdl
        /guard:cf
        /GS
    )

    # Optimization flags for Release builds
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Release>:/Ox>      # Maximum optimization
        $<$<CONFIG:Release>:/Oi>      # Enable intrinsic functions
        $<$<CONFIG:Release>:/Gy>      # Function-level linking
        $<$<CONFIG:Release>:/GL>      # Whole program optimization
    )

    # Linker flags for Release builds
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/LTCG>    # Link-time code generation
        $<$<CONFIG:Release>:/NXCOMPAT>    # Data Execution Prevention
        $<$<CONFIG:Release>:/CETCOMPAT>    # Control-flow Enforcement Technology
        $<$<CONFIG:Release>:/DYNAMICBASE>    # Address Space Layout Randomization
        $<$<CONFIG:Release>:/MACHINE:X64>    # Target machine
    )
endif()

# Finally add any additional libraries
target_link_libraries(${PROJECT_NAME})