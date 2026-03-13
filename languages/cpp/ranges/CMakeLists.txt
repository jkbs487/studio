cmake_minimum_required(VERSION 3.16)
project(ranges_experiment LANGUAGES CXX)

add_executable(ranges main.cpp)
target_compile_features(ranges PRIVATE cxx_std_20)
