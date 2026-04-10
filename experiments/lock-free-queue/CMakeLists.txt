cmake_minimum_required(VERSION 3.16)
project(lock_free_queue_experiment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

add_executable(lock-free-queue main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(lock-free-queue PRIVATE Threads::Threads)