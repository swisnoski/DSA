cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)

project(ChessCpp)

add_executable(ChessCpp ChessCpp.cpp)

target_include_directories(ChessCpp PRIVATE
    "C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/SDL3-3.4.16/include"
)

target_link_libraries(ChessCpp PRIVATE
    "C:/Users/swisnoski/OneDrive - Olin College of Engineering/2026_02 Fall/Data Structures + Algorithms/Assignment 1/ChessCpp/SDL3-3.4.16/lib/x86/SDL3.lib"
)