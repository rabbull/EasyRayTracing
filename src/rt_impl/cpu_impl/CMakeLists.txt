cmake_minimum_required(VERSION 3.18)
project(rt_cpu_impl LANGUAGES C)

add_compile_definitions(DOUBLE_PRECISION)

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

find_package(OpenMP REQUIRED)
message(STATUS "OpenMP:")
message(STATUS "  Found: ${OpenMP_C_FOUND}")
message(STATUS "  Version: ${OpenMP_C_VERSION}")
message(STATUS "  Include Directories: ${OpenMP_C_INCLUDE_DIRS}")
message(STATUS "  Libraries: ${OpenMP_C_LIBRARIES}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(BLAS REQUIRED cblas lapacke)
message(STATUS "BLAS:")
message(STATUS "  Found: ${BLAS_FOUND}")
message(STATUS "  Version:")
message(STATUS "    cblas: ${BLAS_cblas_VERSION}")
message(STATUS "    lapacke: ${BLAS_lapacke_VERSION}")
message(STATUS "  Include Directories: ${BLAS_INCLUDE_DIRS}")
message(STATUS "  Libraries: ${BLAS_LIBRARIES}")

include_directories(include/)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})
include_directories(${OpenMP_C_INCLUDE_DIRS})
include_directories(${BLAS_INCLUDE_DIRS})

file(GLOB SOURCE_FILES *.c include/*.h)

add_library(rt_cpu_impl STATIC ${SOURCE_FILES})
target_link_directories(rt_cpu_impl PUBLIC
        ${Python3_LIBRARY_DIRS}
        ${Python3_NumPy_LIBRARY_DIRS}
        ${OpenMP_C_LIBRARY_DIRS}
        ${BLAS_LIBRARY_DIRS})
target_link_libraries(rt_cpu_impl PUBLIC m
        rt_impl_common
        ${Python3_LIBRARIES}
        ${Python3_NumPy_LIBRARIES}
        ${OpenMP_C_LIBRARIES}
        ${BLAS_LIBRARIES})
