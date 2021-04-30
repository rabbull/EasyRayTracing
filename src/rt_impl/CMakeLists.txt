cmake_minimum_required(VERSION 3.18)
project(rt_impl LANGUAGES C)

add_compile_definitions(DOUBLE_PRECISION)

add_subdirectory(cpu_impl)
#add_subdirectory(cuda_impl)

include_directories(cpu_impl/include)
add_executable(playground main.c)
target_link_libraries(playground rt_cpu_impl)
