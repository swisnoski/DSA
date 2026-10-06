cmake_minimum_required(VERSION 3.10)

project(SortMaster)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(theBIGsort benchmarking.cpp)
