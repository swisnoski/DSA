cmake_minimum_required(VERSION 3.10)

project(GraphMaster)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# okay so we can play around with this in order to test the right functionality 
# if we want to import priority queue into Dijkstra, then we DON'T want to define 
# priorityQueue as an executable since that means we will have duplicate "main"
# functions. So we just comment it and it's main function out. 
# okay and we want to do this for Graphs too 

# add_executable(PriorityQueue PriorityQueue.cpp)
# add_executable(Graph Graphs.cpp)

add_executable(Dijkstra Dijkstra.cpp)
