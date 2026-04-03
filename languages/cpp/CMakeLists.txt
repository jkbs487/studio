cmake_minimum_required(VERSION 3.16)
project(cpp_experiments LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加所有实验
add_subdirectory(templates)
add_subdirectory(smart-pointers)
add_subdirectory(lambda)
add_subdirectory(move-semantics)
add_subdirectory(concurrency)
add_subdirectory(ranges)
add_subdirectory(coroutines)
add_subdirectory(format)
add_subdirectory(std-span)
add_subdirectory(std-expected)
