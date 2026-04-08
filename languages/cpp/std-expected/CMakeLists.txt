cmake_minimum_required(VERSION 3.16)
project(std_expected_experiment LANGUAGES CXX)

add_executable(std-expected main.cpp)
target_compile_features(std-expected PRIVATE cxx_std_23)