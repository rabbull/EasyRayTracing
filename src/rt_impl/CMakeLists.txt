cmake_minimum_required(VERSION 3.18)
project(rt_impl C)

set(ENV{VIRTUAL_ENV} "../../venv")
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

find_package(PkgConfig REQUIRED)
pkg_check_modules(BLAS REQUIRED cblas lapacke)
message(STATUS "BLAS:")
message(STATUS "  Found: ${BLAS_FOUND}")
message(STATUS "  Version:")
message(STATUS "    cblas: ${BLAS_cblas_VERSION}")
message(STATUS "    lapacke: ${BLAS_lapacke_VERSION}")
message(STATUS "  Include Directories: ${BLAS_INCLUDE_DIRS}")
message(STATUS "  Link Directories: ${BLAS_LIBRARY_DIRS}")
message(STATUS "  Libraries: ${BLAS_LIBRARIES}")

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
