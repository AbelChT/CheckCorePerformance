cmake_minimum_required(VERSION 3.15)
project(IsolatedLibTest C)

set(CMAKE_C_STANDARD 11)

add_executable(IsolatedLibTest main.c lib_core_performance_test.c lib_core_performance_test.h)