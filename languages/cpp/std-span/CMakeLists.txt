cmake_minimum_required(VERSION 3.16)
project(std_span_experiment LANGUAGES CXX)

add_executable(std-span main.cpp)
target_compile_features(std-span PRIVATE cxx_std_20)
