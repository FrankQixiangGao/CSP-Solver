cmake_minimum_required(VERSION 3.17)
project(CSP-Solver)

set(CMAKE_CXX_STANDARD 11)

add_executable(AIHW2 main.cpp constraint.cpp constraint.h Variable.cpp Variable.h State.cpp State.h)
