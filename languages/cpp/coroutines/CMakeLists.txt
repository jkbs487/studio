cmake_minimum_required(VERSION 3.16)
project(coroutines_experiment LANGUAGES CXX)

add_executable(coroutines main.cpp)
target_compile_features(coroutines PRIVATE cxx_std_20)

# 需要线程支持（用于异步演示）
find_package(Threads REQUIRED)
target_link_libraries(coroutines PRIVATE Threads::Threads)
