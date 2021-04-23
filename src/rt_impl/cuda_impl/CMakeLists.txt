cmake_minimum_required(VERSION 3.18)
project(rt_impl LANGUAGES C)

set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
enable_language(CUDA)

set(Python3_FIND_VIRTUALENV ONLY)
find_package(Python3 COMPONENTS Interpreter Development NumPy)
message(STATUS "Python3:")
message(STATUS "  Found: ${Python3_FOUND}")
message(STATUS "  Path: ${Python3_EXECUTABLE}")
message(STATUS "  Version:")
message(STATUS "    Python3: ${Python3_VERSION}")
message(STATUS "    NumPy: ${Python3_NumPy_VERSION}")
message(STATUS "  Include Directories: ${Python3_INCLUDE_DIRS}")
message(STATUS "  Link Directories: ${Python3_LIBRARY_DIRS}")
message(STATUS "  Libraries: ${Python3_LIBRARIES} ${Python3_NumPy_LIBRARIES}")

include_directories(../common)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})

file(GLOB SOURCE_FILES *.c *.cu include/*.h)

add_library(rt_cuda_impl SHARED ${SOURCE_FILES})
target_link_directories(rt_cuda_impl PUBLIC
        ${Python3_LIBRARY_DIRS}
        ${Python3_NumPy_LIBRARY_DIRS})
target_link_libraries(rt_cuda_impl PUBLIC m
        rt_impl_common
        ${Python3_LIBRARIES}
        ${Python3_NumPy_LIBRARIES})
