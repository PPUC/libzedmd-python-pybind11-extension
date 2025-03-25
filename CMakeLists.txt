cmake_minimum_required(VERSION 3.17)

set(PLATFORM "win")
set(ARCH "x64")

# Projet name
project(ZeDMD_ext DESCRIPTION "Python cross-platform library for communicating with ZeDMD.")

find_package(pybind11 REQUIRED)

include_directories(
    extern/libzedmd/src
)

add_library(extending MODULE extending.cpp)

target_link_directories(extending PRIVATE
    extern/libzedmd/build
    extern/libzedmd/third-party/build-libs/${PLATFORM}/${ARCH}
)

target_link_libraries(extending PRIVATE
    pybind11::module
    zedmd64
    libserialport64
)

set_target_properties(extending PROPERTIES
    PREFIX ""
    SUFFIX ".pyd"
)


