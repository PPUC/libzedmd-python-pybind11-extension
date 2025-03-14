cmake_minimum_required(VERSION 3.17)

# Projet name
project(extending)

# C++ version to use
set(CMAKE_CXX_STANDARD 11)

# Builtin option
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

# Python3 needed
find_package (Python3 REQUIRED COMPONENTS Interpreter Development)

include_directories(
    ${Python3_INCLUDE_DIRS}
    extern/pybind11/include
    extern/libzedmd/src
)

add_library(extending SHARED extending.cpp)

target_link_directories(extending PRIVATE
    extern/libzedmd/build
)

target_link_libraries(extending
    PRIVATE
    ${Python3_LIBRARIES}  
    dl
    pthread
    util
    zedmd
    serialport
)

set_target_properties(extending PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} 
    PREFIX ""
)


