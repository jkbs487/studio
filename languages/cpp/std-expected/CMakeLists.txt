cmake_minimum_required(VERSION 3.16)
project(std_expected_experiment LANGUAGES CXX)

add_executable(std-expected main.cpp)
set_target_properties(std-expected PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)