cmake_minimum_required(VERSION 3.10)

project(MatrixMaster)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# add_executable(Matrixopia matrix.cpp)
add_executable(MatrixopiaBench matrixBenchmarking.cpp)
add_executable(NeedleHamLunch needlemanWunsch.cpp)
