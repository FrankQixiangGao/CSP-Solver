cmake_minimum_required(VERSION 3.17)
project(CSP_Solver)

set(CMAKE_CXX_STANDARD 11)

add_executable(CSP_Solver main.cpp State.h State.cpp Driver.cpp Driver.h Constraint.cpp Constraint.h)