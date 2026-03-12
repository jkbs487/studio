cmake_minimum_required(VERSION 3.16)
project(move_semantics_experiment LANGUAGES CXX)

add_executable(move-semantics main.cpp)
target_compile_features(move-semantics PRIVATE cxx_std_20)
