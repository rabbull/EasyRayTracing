cmake_minimum_required(VERSION 3.18)
project(rt_impl C)

set(ENV{VIRTUAL_ENV} "../../venv")
set(Python3_FIND_VIRTUALENV ONLY)
find_package(Python3 COMPONENTS Interpreter Development NumPy)
if (${Python3_FOUND} AND ${Python3_NumPy_FOUND})
    message(STATUS "- Python3 Found at: ${Python3_EXECUTABLE}")
    message(STATUS "- NumPy Found, Version: ${Python3_NumPy_VERSION}")
endif ()

find_package(PkgConfig REQUIRED)
pkg_search_module(BLAS REQUIRED mkl-dynamic-lp64-seq)

include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})
include_directories(${BLAS_INCLUDE_DIRS})

file(GLOB SOURCE_FILES src/*.c src/include/*.h)

add_library(rt_impl STATIC ${SOURCE_FILES})
target_link_directories(rt_impl PUBLIC ${BLAS_LIBRARY_DIRS})
target_link_libraries(rt_impl PUBLIC
        ${Python3_LIBRARIES}
        ${Python3_NumPy_LIBRARIES}
        ${BLAS_LIBRARIES})

add_executable(rt_impl_test main.c)
target_link_libraries(rt_impl_test rt_impl)
