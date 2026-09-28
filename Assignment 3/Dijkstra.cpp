/* 
Samuel Wisnoski 
9/26/2026

Step three: Searching with Dijkstra!  
*/

#include <vector>
#include <iostream>
#include <set> 
#include <string>
#include <limits>
#include <fstream>
#include <filesystem>
using namespace std; 

// we will also be seeing a lot of infinity so let's make it a constant 
const double INF = std::numeric_limits<double>::infinity();

// ################ PART THREEE: MAKE A DIJKSTRA'S ALGORITHM #####################

/* 
Algorithm pseduocode from day 7

for each vertex, v that is not the source:
    prev[v] ← UNDEFINED
    dist[v] ← INFINITY
    queue.addWithPriority(v, INFINITY)

dist[source] ← 0
queue.addWithPriority(source, 0)

while queue is not empty:
    u ← vertex in queue with min priority
    remove u from queue
    for each neighbor v of u still in queue:
        alt ← dist[u] + edgecost(u, v)
        if alt < dist[v]:
            dist[v] ← alt
            queue.changePriority(v, alt)
            prev[v] ← u
// reconstruct shortest path from prev
*/

#include "PriorityQueue.cpp"
#include "Graphs.cpp"

// so basically how we can set this up is if we have a graph that holds a map of vertices 
// of a selected vertex type, and a priority queue that has type vertextype and whatever priority we give it 

template <typename VertexType>
class Dijkstra{
    public:
    // so we hold a graph that we can reset at any time 
    Graph<VertexType> dijkstraGraph;

    // so we want to return a vector for the shortest path 
    std::vector<VertexType> findShortestPath(VertexType source, VertexType end){
        // we start by collecting eact of the vertices 
        std::set<VertexType> vertexSet = dijkstraGraph.getVertices();

        // we also want a minPriorityQueue
        MinPriorityQueue<VertexType> minQueue;

        // as well as a map to the previous lowest vertex and the shortest distance 
        // from source to vertex. 
        std::map<VertexType, double> dist;
        std::map<VertexType, VertexType> prev;

        // we also need to track which vertices we've already gone through
        std::set<VertexType> remainingVertices;
        
        // Now we are ready to dijkstra's! 
        // first, we loop through each of the vertices in our set of vertices by
        // using a for loop 
        for (VertexType vertex : vertexSet) {
            //update our maps 
            // prev[v] ← UNDEFINED // we can just not define prev until we start walking through the edges 
            dist[vertex] = INF; //infinity and beyond 

            minQueue.addWithPriority(vertex, INF);
            remainingVertices.insert(vertex);
        }

        // we can add the source (passed in to the function) to the map, which will always have a distance of 
        // 0. Again, no previous, since it's the source 
        dist[source] = 0;
        minQueue.adjustPriority(source, 0);

        // Now, we are all set up for part two! Copy and pasting pseudocode for reference
        // while queue is not empty:
        // u ← vertex in queue with min priority
        // remove u from queue
        // for each neighbor v of u still in queue:
        //     alt ← dist[u] + edgecost(u, v)
        //     if alt < dist[v]:
        //         dist[v] ← alt
        //         queue.changePriority(v, alt)
        //         prev[v] ← u
        // reconstruct shortest path from prev

        while(!minQueue.isEmpty()){
            // next pops the next vertex and retrieves it 
            VertexType nextVertex = minQueue.next();
            remainingVertices.erase(nextVertex); // we use erase to remove an element from a set 

            std::map<VertexType, double> nextVertexNeighbors = dijkstraGraph.getEdges(nextVertex);
            for(auto [vertex, cost] : nextVertexNeighbors){
                if (remainingVertices.find(vertex) != remainingVertices.end()) { //for each neighbor v of u still in queue:
                    double alt = dist[nextVertex] + cost; //alt ← dist[u] + edgecost(u, v)
                    if (alt < dist[vertex]){ //if alt < dist[v]:
                        dist[vertex] = alt; //dist[v] ← alt
                        prev[vertex] = nextVertex; //prev[v] ← u
                        minQueue.adjustPriority(vertex, alt); //queue.changePriority(v, alt)
                    }
                }
            }
        }
        
        // reconstruct shortest path from prev map
        std::vector<VertexType> path;

        // if there is no shortest path, the distance to end will still 
        // be infinity 
        if (dist[end] == INF) {
            return {};
        }

        // otherwise, return shortest path 
        // start at the end of the path 
        VertexType pathStep = end;
        while (pathStep != source) { // while we haven't gotten to the source 
            path.push_back(pathStep); // add the last step and move forward 
            pathStep = prev[pathStep];
        }
        // the last step will always be the source 
        path.push_back(source);
        return path;
    
    }
};

// test functions for Dijstra's are located at the bottom of the file 



// ############### FINAL PART: https://projecteuler.net/problem=83 #################

// problems 81, 82, and 83 are all relatively similar, 83 specifically allows you to move up down, left, and right to find your way from top left to bottom right. 

// specifically, instructions: Find the minimal path sum from the top left to the bottom right by moving left, right, up, and down in matrix.txt 
// (right click and "Save Link/Target As..."), a 31K text file containing an 80 by 80 matrix.

// so the tricky part of this is loading the test file as a matrix, and then converting that into a meaningful graph.
// however, once we actually create the graph correctly, we can simply use our dijkstra class to solve the problem 

// Apparently, the expected answer is 425185


void solveProblem83() {

    // we can first load the file as a text file which parses by line using ifstream and the fstream library 
    std::ifstream file("0083_matrix.txt");

    // we know the matrix is 80 by 80 so we can repeatedly use an int 
    const int matrixSize = 80;

    // so we can declare the matrix of size 80 x 80 
    int matrix[matrixSize][matrixSize];

    // and then we can loop through the rows of our matrix and 
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            file >> matrix[row][col]; // ok so I had to pull this line from the internet, but essentially it works like this :
            // because our matrix is a matrix of integers, the file knows to insert integers. We also ignore "," below, which 
            // is what actually seperates the integers in the file.  the >> symbol functions as telling us to load the "next" piece 
            // of data, which, because we iterate by rows then columns, slots perfectly into our matrix. 

            if (col < matrixSize - 1) {
                file.ignore(1, ',');
            }
        }
    }

    // now we need to make our actual graph, which we can do directly through our 
    // dijkstra class 
    Dijkstra<string> solver;

    // to turn each cell into a graph vertex, we can then interate (again) through each int in our matrix, 
    // and map each row and column to a string such as "(0,0)" or "(19,43)". This helps us keep track of the grid 
    // then, we check each row has a left/right/up/down neighbor within the bounds of the graph. if it does, we simply 
    // check the value of that neighbor and assign the value of that cell as the cost to walk that edge. That way, 
    // moving "into" the cell (or including it on the path) allows us to maximize for the lowest valued cells 
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {

            string current = "(" + to_string(row) + "," + to_string(col) + ")";

            // check down adding one to row 
            if (row + 1 < matrixSize) {
                string neighbor = "(" + to_string(row + 1) + "," + to_string(col) + ")";
                solver.dijkstraGraph.addEdge(current, neighbor, matrix[row + 1][col]);
            }

            // check up by subtracting one from row 
            if (row - 1 >= 0) {
                string neighbor = "(" + to_string(row - 1) + "," + to_string(col) + ")";
                solver.dijkstraGraph.addEdge(current, neighbor, matrix[row - 1][col]);
            }

            // check right by adding one to column 
            if (col + 1 < matrixSize) {
                string neighbor = "(" + to_string(row) + "," + to_string(col + 1) + ")";
                solver.dijkstraGraph.addEdge(current, neighbor, matrix[row][col + 1]);
            }

            // check left by subtracting one from column 
            if (col - 1 >= 0) {
                string neighbor = "(" + to_string(row) + "," + to_string(col - 1) + ")";
                solver.dijkstraGraph.addEdge(current, neighbor, matrix[row][col - 1]);
            }
        }
    }

    // now we have our full cost, and we can return the shortest path between the top left 
    // and the botom right 
    vector<string> shortestPath = solver.findShortestPath("(0,0)", "(79,79)");

    int totalCost = 0;

    // and lastly, we need to calculate cost. we iterate through each vertex in the shortest path 
    // and append it's matrix value to the total cost 
    for (string vertex : shortestPath) {
        int row;
        int col;

        sscanf(vertex.c_str(), "(%d,%d)", &row, &col);
        // okay admittedly I AIed this line because I figured it would be easier 
        // c_string converts into a string that sscanf can understand since it's an older 
        // c function. (%d,%d) is just regex expression, and of course &row and &col just tell 
        // where to store the values 

        totalCost += matrix[row][col];
    }

    std::cout << "Expected Total Cost: 425185\n";
    std::cout << "Total Cost: " << totalCost << "\n";
}





// and last but not least, we can invoke the power of AI to write some tests for us!!
template <typename T>
void printPath(const std::string& testName, const std::vector<T>& path) {
    std::cout << testName << ": ";
    if (path.empty()) {
        std::cout << "No path found\n";
        return;
    }
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i + 1 < path.size() ? " -> " : "\n");
    }
}



void testDijkstra() {
    // ----------------------------------------------------
    // Test 1: Simple Linear Graph (A -> B -> C)
    // ----------------------------------------------------
    {
        Dijkstra<string> solver;
        // Adding directed edges with weights
        solver.dijkstraGraph.addEdge("A", "B", 3.0);
        solver.dijkstraGraph.addEdge("B", "C", 2.0);

        vector<string> path = solver.findShortestPath("A", "C");
        std::cout << "Expected Path: A -> B -> C\n";
        printPath("Test 1 (Linear Graph)", path);
    }

    // ----------------------------------------------------
    // Test 2: Diamond Graph (Chooses shorter weight route)
    // Path 1: A -> B -> D (cost: 5 + 5 = 10)
    // Path 2: A -> C -> D (cost: 1 + 2 = 3)  <-- Should pick this
    // ----------------------------------------------------
    {
        Dijkstra<string> solver;
        solver.dijkstraGraph.addEdge("A", "B", 5.0);
        solver.dijkstraGraph.addEdge("B", "D", 5.0);
        
        solver.dijkstraGraph.addEdge("A", "C", 1.0);
        solver.dijkstraGraph.addEdge("C", "D", 2.0);

        vector<string> path = solver.findShortestPath("A", "D");
        std::cout << "Expected Path: A -> C -> D\n";
        printPath("Test 2 (Shorter Route)", path);
    }

    // ----------------------------------------------------
    // Test 3: Dense Multi-Path Network (7 Nodes)
    // Tests overriding a longer multi-hop path with a shorter direct/indirect sequence
    // Path 1: Start -> A -> B -> End (cost: 2 + 2 + 10 = 14)
    // Path 2: Start -> C -> D -> E -> End (cost: 1 + 1 + 1 + 1 = 4) <-- Should pick this
    // ----------------------------------------------------
    {
        Dijkstra<string> solver;
        solver.dijkstraGraph.addEdge("Start", "A", 2.0);
        solver.dijkstraGraph.addEdge("A", "B", 2.0);
        solver.dijkstraGraph.addEdge("B", "End", 10.0);

        solver.dijkstraGraph.addEdge("Start", "C", 1.0);
        solver.dijkstraGraph.addEdge("C", "D", 1.0);
        solver.dijkstraGraph.addEdge("D", "E", 1.0);
        solver.dijkstraGraph.addEdge("E", "End", 1.0);

        vector<string> path = solver.findShortestPath("Start", "End");
        std::cout << "Expected Path: Start -> C -> D -> E -> End (Total Cost: 4.0)\n";
        printPath("Test 3 (Dense Multi-Path Graph)", path);
    }

    // ----------------------------------------------------
    // Test 4: 3x3 Grid Network (Connected Mesh)
    // Tests grid traversal where multiple paths exist with varying edge weights
    // Nodes: (0,0) to (2,2)
    // Path (0,0) -> (0,1) -> (0,2) -> (1,2) -> (2,2) = 1+1+1+1 = 4
    // Path (0,0) -> (1,0) -> (2,0) -> (2,1) -> (2,2) = 5+5+1+1 = 12
    // ----------------------------------------------------
    {
        Dijkstra<string> solver;
        // Row 0 connections
        solver.dijkstraGraph.addEdge("(0,0)", "(0,1)", 1.0);
        solver.dijkstraGraph.addEdge("(0,1)", "(0,2)", 1.0);

        // Row 1 connections
        solver.dijkstraGraph.addEdge("(1,0)", "(1,1)", 2.0);
        solver.dijkstraGraph.addEdge("(1,1)", "(1,2)", 2.0);

        // Row 2 connections
        solver.dijkstraGraph.addEdge("(2,0)", "(2,1)", 1.0);
        solver.dijkstraGraph.addEdge("(2,1)", "(2,2)", 1.0);

        // Vertical connections
        solver.dijkstraGraph.addEdge("(0,0)", "(1,0)", 5.0);
        solver.dijkstraGraph.addEdge("(1,0)", "(2,0)", 5.0);

        solver.dijkstraGraph.addEdge("(0,1)", "(1,1)", 3.0);
        solver.dijkstraGraph.addEdge("(1,1)", "(2,1)", 3.0);

        solver.dijkstraGraph.addEdge("(0,2)", "(1,2)", 1.0);
        solver.dijkstraGraph.addEdge("(1,2)", "(2,2)", 1.0);

        vector<string> path = solver.findShortestPath("(0,0)", "(2,2)");
        std:cout << "Expected Path: (0,0) -> (0,1) -> (0,2) -> (1,2) -> (2,2) (Total Cost: 4.0)\n";
        printPath("Test 4 (3x3 Grid Network)", path);
    }

    // ----------------------------------------------------
    // Test 5: Graph with Cycles and Alternative Loops
    // Tests that cycles do not cause infinite loops and priority queue correctly updates
    // ----------------------------------------------------
    {
        Dijkstra<string> solver;
        // Main backbone
        solver.dijkstraGraph.addEdge("A", "B", 4.0);
        solver.dijkstraGraph.addEdge("B", "C", 3.0);
        solver.dijkstraGraph.addEdge("C", "D", 2.0);

        // Cycles/Loops back
        solver.dijkstraGraph.addEdge("B", "A", 1.0);
        solver.dijkstraGraph.addEdge("C", "B", 1.0);

        // Shortcut bypasses
        solver.dijkstraGraph.addEdge("A", "C", 8.0);
        solver.dijkstraGraph.addEdge("B", "D", 4.0); // A -> B -> D = 4 + 4 = 8

        vector<string> path = solver.findShortestPath("A", "D");
        std::cout << "Expected Path: A -> B -> D\n";
        printPath("Test 5 (Graph with Cycles)", path);
    }

    // ----------------------------------------------------
    // Test 6: No Path Exists
    // Tests that Dijkstra correctly handles disconnected vertices
    // ----------------------------------------------------
    {
        Dijkstra<string> solver;

        solver.dijkstraGraph.addEdge("A", "B", 1.0);
        solver.dijkstraGraph.addEdge("B", "C", 2.0);

        // D is not connected to A
        solver.dijkstraGraph.addEdge("D", "E", 1.0);

        vector<string> path = solver.findShortestPath("A", "D");

        std::cout << "Expected Path: No path found\n";
        printPath("Test 6 (No Path)", path);
    }
}


int main() {
    testDijkstra();
    solveProblem83();

    return 0;
}