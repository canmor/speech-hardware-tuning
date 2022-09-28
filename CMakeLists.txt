cmake_minimum_required(VERSION 3.19)
project(bench)

set(CMAKE_CXX_STANDARD 17)

include(gtest.cmake)
include(benchmark.cmake)

add_executable(bench)
target_sources(bench PRIVATE cache.cpp)
target_link_libraries(bench PRIVATE benchmark::benchmark_main)
target_compile_options(bench PRIVATE -fno-tree-vectorize)