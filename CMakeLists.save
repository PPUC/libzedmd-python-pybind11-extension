cmake_minimum_required(VERSION 3.17)

set(PLATFORM "win" CACHE STRING "Platform")
set(ARCH "x64" CACHE STRING "Arch")
set(PYTHON_INCLUDE_DIR "?" CACHE STRING "PYTHON_INCLUDE_DIR")
set(PYTHON_LIBRARY "?" CACHE STRING "PYTHON_LIBRARY")
set(WIN64 "")
set(LIBWIN "")

message(STATUS "PLATFORM: ${PLATFORM}")
message(STATUS "ARCH: ${ARCH}")

if(PLATFORM STREQUAL "macos")
   set(CMAKE_OSX_DEPLOYMENT_TARGET 14.0)
   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
   endif()
endif()



# Projet name
project(extending DESCRIPTION "Python cross-platform library for communicating with ZeDMD.")

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
#   set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#   set(CMAKE_INSTALL_RPATH "@executable_path")
#elseif(PLATFORM STREQUAL "linux")
#   set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#   set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()

# C++ version to use
set(CMAKE_CXX_STANDARD 11)


#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
#set(CMAKE_C_VISIBILITY_PRESET hidden)


# Builtin option
set(CMAKE_CXX_FLAGS_RELEASE "-O3 ")

# Python3 needed
if (NOT ((PLATFORM STREQUAL "win") AND (ARCH STREQUAL "x86")))
  find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
else()
  set(Python3_INCLUDE_DIRS "${PYTHON_INCLUDE_DIR}")
  set(Python3_LIBRARIES "${PYTHON_LIBRARY}")
  # set(Python3_INCLUDE_DIRS "C:/hostedtoolcache/windows/Python/3.9.13/x64/Include")
endif()

message(STATUS "Python3_INCLUDE_DIRS=${Python3_INCLUDE_DIRS}")
message(STATUS "Python3_LIBRARIES=${Python3_LIBRARIES}")

include_directories(
    ${Python3_INCLUDE_DIRS}
    extern/pybind11/include
    extern/libzedmd/src
)

add_library(extending SHARED extending.cpp)

target_link_directories(extending PRIVATE
    extern/libzedmd/build
    extern/libzedmd/third-party/build-libs/${PLATFORM}/${ARCH}
)

target_link_libraries(extending PRIVATE
    ${Python3_LIBRARIES}  
    zedmd${WIN64}
    ${LIBWIN}serialport${WIN64}
)

set_target_properties(extending PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} 
    PREFIX ""
)


