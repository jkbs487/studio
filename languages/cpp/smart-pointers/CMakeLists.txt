cmake_minimum_required(VERSION 3.16)
project(smart_pointers_experiment LANGUAGES CXX)

add_executable(smart-pointers main.cpp)
target_compile_features(smart-pointers PRIVATE cxx_std_20)
