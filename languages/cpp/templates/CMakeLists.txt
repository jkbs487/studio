cmake_minimum_required(VERSION 3.16)
project(templates_experiment LANGUAGES CXX)

add_executable(templates main.cpp)
target_compile_features(templates PRIVATE cxx_std_20)
