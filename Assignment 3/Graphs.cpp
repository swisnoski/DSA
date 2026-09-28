/* 
Samuel Wisnoski 
9/20/2026

Graph time baby let's go all the way to graph time graph time WOOOOO
*/


#include <vector>
#include <iostream>
#include <set> 
#include <map>
#include <string>
using namespace std; 



// ################ PART ONE: MAKE A GRAPH #####################

/* 
``Graph`` represents a directed graph
@param VertexType the type that represents a vertex in the graph

interface Graph<VertexType> {
    @return the vertices in the graph
    fun getVertices(): Set<VertexType>

    Add an edge between [from] and [to] with edge weight [cost]
    fun addEdge(from: VertexType, to: VertexType, cost: Double)

    Get all the edges that begin at [from]
    @return a map where each key represents a vertex connected to [from] and the value represents the edge weight.
    fun getEdges(from: VertexType): Map<VertexType, Double>

    Remove all edges and vertices from the graph
    fun clear()
}
*/

// because this is a directed graph, we only care about/track the outward connections for a node, so we intrinsically know the 
// "from" node for each edge, so we only need to track the "to" vertex 

// so to get verticies, we just return the set of vertices
// to add an edge, we need to declare two vertices, pass two vertices, and add the edge weight 

// we can mimic the structure of the graph shown in day 6 

/**
 * Represents a directed weighted graph.
 * @param VertexType the type that represents a vertex in the graph
 */
template <typename VertexType> // let's make another function that is kinda like an interface 
class Graph{
    private:
    std::map<VertexType, std::map<VertexType, double>> adjacencyMap; // except we want the edges to have a cost 
    // so instead of having a map that maps a vertex to a set of vertecies, we want a map that maps a vertex to another 
    // map that maps all the vertecies it's connected to to a set cost. 

    // so each entry of this map looks like : 
    // Vertex A mapped to { 
    //  - Vertex B mapped to cost 10
    //  - Vertex C mapped to cost 3
    //  - Vertex n mapped to cost c}

    // and then this map has a mapping for each vertex that connects to all it's adjacent verticies 
    // we will therefore call this map our "adjacencyMap"

    public:
    /**
     * Gets all vertices in the graph.
     * @return the vertices in the graph
     */
    // @return the vertices in the graph
    std::set<VertexType> getVertices(){
        // so return all the vertices, we can create a set of vertices 
        std::set<VertexType> vertices;

        // then we can loop through all of the items in our adjacency map, which has 
        // one item per vertex, and just extract the vertex from the mapping 
        for (auto pair : adjacencyMap) {
            vertices.insert(pair.first);
        }

        return vertices;
    }
    
    /**
     * Adds a vertex to the graph with no connections.
     * @param nodeData the vertex to add
     * @return true if added, false if vertex already exists
     */
    // so this function is not in the HW assignment but was in the inclass assignment. 
    // it seems useful so we are going to keep it
    // to add a vertex we just need to add an addition to the adjacency map with no connections
    bool addVertex(VertexType nodeData) {
        // if it's already in the map, we don't do anything 
        if (adjacencyMap.find(nodeData) != adjacencyMap.end()) {    //note: adjacencyMap.end is what's returned by find if the map
                                                                    // does not contain the data. so if we return anything else, then the map DOES contain it
            return false; // Node already exists
        }
        // if we don't already have it, we map the nodedata to an empty map of type map<VertexType, double>
        adjacencyMap[nodeData] = std::map<VertexType, double>();
        return true; // and then return 0 anyway 
    }

    /**
     * Adds an edge between two vertices with the given cost.
     * @param fromVertex source vertex
     * @param toVertex target vertex
     * @param cost edge weight
     * @return true if the edge was added
     */
    // Add an edge between [fromVertex] and [toVertex] with edge weight [cost]
    bool addEdge(VertexType fromVertex, VertexType toVertex, double cost) {
        // first, let's make sure the vertecies are in the graph 
        addVertex(fromVertex);
        addVertex(toVertex);
        // of they are already in the graph, nothing happens  

        // then we can find the in the adjacency list and grab their ENTIRE entry, key and value 
        auto selectedfromVertex = adjacencyMap.find(fromVertex);

        // lastly, we create the edge by accesing the second item (the value, ie, the map of vertex -> cost)
        // and assigning (or reassigning) the pair of selected vertex -> cost 
        selectedfromVertex->second[toVertex] = cost;
        return true;
    }

    /**
     * Gets all edges that begin at the given vertex.
     * @param fromVertex source vertex
     * @return a map where each key is a connected vertex and value is edge weight
     */
    // @return a map where each key represents a vertex connected to [from] and the value represents the edge weight.
    // Returns a map where key = target vertex, value = edge cost
    std::map<VertexType, double> getEdges(VertexType fromVertex) const {
        auto selectedVertex = adjacencyMap.find(fromVertex);
        if (selectedVertex != adjacencyMap.end()) { // as long as it exists 
            return selectedVertex->second; // just return the existing edge mapping 
        }
        return {}; // Return empty map if vertex doesn't exist
    }

    /**
     * Removes all edges and vertices from the graph.
     */
    // Remove all edges and vertices from the graph
    void clearGraph() {
        adjacencyMap.clear();
    }
};


/**
 * Tests Graph functionality.
 */
 // and then let's test that functionality 
void testGraph() {
    std::cout << "\nGraph Tests\n";
    Graph<std::string> graph;

    // Test initial empty state
    std::cout << "getVertices on new graph (expected 0): " << graph.getVertices().size() << "\n";
    std::cout << "getEdges on missing vertex 'A' (expected 0): " << graph.getEdges("A").size() << "\n";

    // Test addVertex
    std::cout << "addVertex('A') (expected 1): " << graph.addVertex("A") << "\n";
    std::cout << "addVertex('A') duplicate (expected 0): " << graph.addVertex("A") << "\n";
    std::cout << "getVertices count after addVertex (expected 1): " << graph.getVertices().size() << "\n";

    // Test addEdge
    std::cout << "addEdge('A', 'B', 10.5) (expected 1): " << graph.addEdge("A", "B", 10.5) << "\n";
    std::cout << "addEdge('A', 'C', 3.2) (expected 1): " << graph.addEdge("A", "C", 3.2) << "\n";
    std::cout << "getVertices count after addEdge (expected 3): " << graph.getVertices().size() << "\n"; // 'A', 'B', 'C'

    // Test getEdges
    auto edgesFromA = graph.getEdges("A");
    std::cout << "getEdges('A') count (expected 2): " << edgesFromA.size() << "\n";
    std::cout << "edge cost A -> B (expected 10.5): " << edgesFromA["B"] << "\n";
    std::cout << "edge cost A -> C (expected 3.2): " << edgesFromA["C"] << "\n";

    auto edgesFromB = graph.getEdges("B");
    std::cout << "getEdges('B') count (expected 0): " << edgesFromB.size() << "\n";

    // Test updating an existing edge weight
    std::cout << "addEdge('A', 'B', 15.0) update (expected 1): " << graph.addEdge("A", "B", 15.0) << "\n";
    std::cout << "updated edge cost A -> B (expected 15): " << graph.getEdges("A")["B"] << "\n";

    // Test clear
    graph.clearGraph();
    std::cout << "getVertices count after clear (expected 0): " << graph.getVertices().size() << "\n";
    std::cout << "getEdges('A') count after clear (expected 0): " << graph.getEdges("A").size() << "\n";
}



// comment out since used as import
// int main() {
//     testGraph();
// }
