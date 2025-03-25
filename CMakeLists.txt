cmake_minimum_required(VERSION 3.17)

set(PLATFORM "win" CACHE STRING "Platform")
set(ARCH "x64" CACHE STRING "Arch")
message(STATUS "PLATFORM: ${PLATFORM}")
message(STATUS "ARCH: ${ARCH}")

# Projet name
project(ZeDMD_ext DESCRIPTION "Python cross-platform library for communicating with ZeDMD.")

set(PYBIND11_FINDPYTHON ON) 
find_package(pybind11 REQUIRED)


set(WIN64 "")
set(LIBWIN "")
if(PLATFORM STREQUAL "win")
  if(ARCH STREQUAL "x86")
    add_compile_definitions(WIN32)
  elseif(ARCH STREQUAL "x64")
    set(WIN64 "64")
  endif()
  set(LIBWIN "lib")
elseif(PLATFORM STREQUAL "macos")
  if (ARCH STREQUAL "arm64")
    set(CMAKE_OSX_ARCHITECTURES arm64)
  elseif(ARCH STREQUAL "x64")
    set(CMAKE_OSX_ARCHITECTURES x86_64)
  endif()
endif()


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
    zedmd${WIN64}
    ${LIBWIN}serialport${WIN64}
)

set_target_properties(extending PROPERTIES
    PREFIX ""
    SUFFIX ".pyd"
)

