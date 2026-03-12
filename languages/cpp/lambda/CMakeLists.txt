cmake_minimum_required(VERSION 3.16)
project(lambda_experiment LANGUAGES CXX)

add_executable(lambda main.cpp)
target_compile_features(lambda PRIVATE cxx_std_20)

# 需要线程支持
find_package(Threads REQUIRED)
target_link_libraries(lambda PRIVATE Threads::Threads)
