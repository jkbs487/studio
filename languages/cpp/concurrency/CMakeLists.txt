cmake_minimum_required(VERSION 3.16)
project(concurrency_experiment LANGUAGES CXX)

add_executable(concurrency main.cpp)
target_compile_features(concurrency PRIVATE cxx_std_20)

# 需要线程支持
find_package(Threads REQUIRED)
target_link_libraries(concurrency PRIVATE Threads::Threads)
