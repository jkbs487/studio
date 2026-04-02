cmake_minimum_required(VERSION 3.16)
project(format_experiment LANGUAGES CXX)

add_executable(format main.cpp)
target_compile_features(format PRIVATE cxx_std_20)
