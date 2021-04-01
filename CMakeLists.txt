cmake_minimum_required(VERSION 3.17)
project(CSP_Solver)

set(CMAKE_CXX_STANDARD 11)

add_executable(CSP_Solver main.cpp State.h State.cpp Constraint.cpp Constraint.h Variable.cpp Variable.h main.h)