cmake_minimum_required(VERSION 3.18)
project(rt_impl C)

set(ENV{VIRTUAL_ENV} "../../venv")
set(Python3_FIND_VIRTUALENV ONLY)
find_package(Python3 COMPONENTS Interpreter Development NumPy)

if (${Python3_FOUND} AND ${Python3_NumPy_FOUND})
    message(STATUS "- Python3 Found at: ${Python3_EXECUTABLE}")
    message(STATUS "- NumPy Found, Version: ${Python3_NumPy_VERSION}")
endif ()

find_package(OpenBLAS CONFIG REQUIRED)

include_directories(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
add_library(rt_impl interface.c ray_tracing.c)
target_link_libraries(rt_impl PRIVATE
        ${Python3_LIBRARIES}
        ${Python3_NumPy_LIBRARIES}
        OpenBLAS::OpenBLAS)
