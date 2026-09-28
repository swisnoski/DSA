/* 
Samuel Wisnoski 
9/24/2026

Okay it's probably time I start making multiple files for different things 

Step two: MinHeap and PriorityQueue! 
*/


#include <vector>
#include <iostream>
#include <set> 
#include <map>
#include <string>
using namespace std; 


// ################ PART TWO: MAKE A PRIORITY QUEUE #####################

/* 
let's make a priority queue! 

but first, let's make a minheap! 

from wikipedia, and also suggested by Paul, (https://en.wikipedia.org/wiki/Heap_(data_structure))
we can learn about how to store a heap in an array 

For a binary heap, the array stores the elements of the tree level by level.
The first index contains the root, the next two indices contain its children,
the next four indices contain the children of those two nodes, and so on.
For a 0-based array, a node at index i has:
   Left child:  2*i + 1
   Right child: 2*i + 2
   Parent:      (i - 1) / 2
This simple indexing scheme makes it efficient to move "up" or "down" the tree.

The four operations we need to make for our heap are: 
add node 
    - add node to end of array then bubble up until it's greater than it's parent and less than it's children 
remove min 
    - remove first node, move last node to root, bubble down until new node fits in graph
bubble down
    - while node is greater than either of it's childen, swap with child until it is less than both children 
bubble up
    - while node is less than it's parent, swap node until not less than it's parent

then, once we define a min heap, we can easily stack our priority queue on top 
*/


// okay so I did this whole minHeap thing and then I realized that the priority queue needs data AND 
// priority in it, so we need to rewrite our minHeap to store data as well. I think we can do this pretty simply 
// by using nodes 

// we can define our own data type with a struct, which is a sort of class (I think?) except 
// it just has data 


/**
 * Node stored in the min-heap.
 * @param heapDataType the type of data stored in the node
 */
template <typename heapDataType>
struct HeapNode {
    heapDataType data;
    double priority;
};

/**
 * Min-heap storing HeapNodes ordered by priority.
 * @param heapDataType the type of data stored in the heap
 */
template <typename heapDataType>
class minHeap{
    private:
    std::vector<HeapNode<heapDataType>> MinHeapArray;
    // so instead of a vector of just doubles we can instead declare a vector of heap nodes 

    public:
    /**
     * Adds a node to the heap and bubbles it up.
     * @param newData data to store
     * @param newPriority priority value, lower means higher priority
     */
    void addNode(heapDataType newData, double newPriority){
        // add the new data to the end of the array 
        // std::cout <<" adding node "; debug

        //declare new node 
        HeapNode<heapDataType> newNode;
        newNode.data = newData;
        newNode.priority = newPriority;

        //add node to end of list 
        MinHeapArray.push_back(newNode);

        // bubble up node based on priority 
        if(MinHeapArray.size()!= 1){
            bubbleUp(MinHeapArray.size()-1);
        }
        return;
    }

    /**
     * Removes and returns the minimum-priority node.
     * @return the HeapNode with smallest priority
     */
    HeapNode<heapDataType> popMin(){
        // std::cout <<" poppin node ";
        if(MinHeapArray.size() == 0){
            return HeapNode<heapDataType>();
        }
        else if(MinHeapArray.size() == 1){
            HeapNode<heapDataType> minNode = MinHeapArray[0];
            MinHeapArray.pop_back();
            return minNode;
        }
        HeapNode<heapDataType> minNode = MinHeapArray[0];
        MinHeapArray[0] = MinHeapArray[MinHeapArray.size()-1];
        MinHeapArray.pop_back();
        bubbleDown(0);
        return minNode;
    }

    /**
     * Moves the node down until heap property is restored.
     * @param nodePosition index of node to bubble down
     */
    void bubbleDown(int nodePosition){
        // needs to handle anything for 1+ nodes 
        // std::cout <<" bubblin down "; debug

        HeapNode<heapDataType> dataNode = MinHeapArray[nodePosition];
        HeapNode<heapDataType> leftChildNode;
        HeapNode<heapDataType> rightChildNode;

        while (true){

            int leftChildIndex = 2*nodePosition + 1;
            int rightChildIndex = 2*nodePosition + 2;

            bool rightExists = (rightChildIndex < MinHeapArray.size());
            bool leftExists = (leftChildIndex < MinHeapArray.size());

            // if both left and right are empty (no children)
            if (!rightExists && !leftExists) {
                break;
            }

            // if the left child exists but the right child doesn't
            if (!rightExists) {
                // we then check if we need to swap 
                leftChildNode = MinHeapArray[leftChildIndex];
                if (dataNode.priority > leftChildNode.priority) {
                    
                    // and then we can swap if needed 
                    MinHeapArray[nodePosition] = leftChildNode;
                    MinHeapArray[2 * nodePosition + 1] = dataNode;
                }
                break;
            }

            // if both children exist, we need to find which child is smaller 
            leftChildNode = MinHeapArray[leftChildIndex];
            rightChildNode = MinHeapArray[rightChildIndex];

            if (leftChildNode.priority < rightChildNode.priority) {
                // if left is smaller, then we check if a swap is needed with the left 
                if (dataNode.priority > leftChildNode.priority) {
                    MinHeapArray[nodePosition] = leftChildNode;
                    MinHeapArray[2 * nodePosition + 1] = dataNode;
                    nodePosition = 2 * nodePosition + 1;
                }
                else {
                    // if it's smaller than left and right we stop 
                    break;
                }
            }
            else{
                // if right is smaller or same size, we check if a swap is needed for the right 
                if (dataNode.priority > rightChildNode.priority) {
                    MinHeapArray[nodePosition] = rightChildNode;
                    MinHeapArray[2 * nodePosition + 2] = dataNode;
                    nodePosition = 2 * nodePosition + 2;
                }
                else {
                    // if it's smaller than left and right we stop 
                    break;
                }
            }
        }
    }

    /**
     * Moves the node up until heap property is restored.
     * @param nodePosition index of node to bubble up
     */
    void bubbleUp(int nodePosition){
        // std::cout <<" bubblin up ";
        HeapNode<heapDataType> dataNode = MinHeapArray[nodePosition];
        HeapNode<heapDataType> parentNode = MinHeapArray[(nodePosition - 1)/2];

        while(dataNode.priority < parentNode.priority){
            // swap values 
            MinHeapArray[nodePosition] = parentNode;
            MinHeapArray[(nodePosition - 1)/2] = dataNode;

            // find new parent and reset node position 
            nodePosition = (nodePosition - 1)/2;
            parentNode = MinHeapArray[(nodePosition - 1)/2];
        }
    }

    /**
     * Checks if the heap is empty.
     * @return true if empty, false otherwise
     */
    // okay so we need to go back and add in this function to actually make the priority queue work 
    bool isEmpty(){
        return (MinHeapArray.size() == 0);
    }

    /**
     * Adjusts the priority of the given element.
     * @param data element whose priority should change
     * @param newPriority the new priority, lower means earlier in order
     */
    // okay so we ALSO need to add an adjust priority function 
    void adjustPriority(heapDataType data, double newPriority) {
        // no easy way to access data in vector so we can just loop through each element in the vector 
        // and then check if it's a match. kinda lengthy but whatever 
        for (int i = 0; i < MinHeapArray.size(); i++) {
            if (MinHeapArray[i].data == data) {
                // once we find a match we swap priority wherever the match was 
                double oldPriority = MinHeapArray[i].priority;
                MinHeapArray[i].priority = newPriority;

                // and then we can check if we need to bubble up or bubble down 
                if (newPriority < oldPriority) {
                    bubbleUp(i);
                }
                else if (newPriority > oldPriority) {
                    bubbleDown(i);
                }
            }
        }   
    }
};

/**
 * Tests MinHeap functionality.
 */
 // and then let's test that functionality 
// (thank you AI for helping me rewrite tests because I just had to majorily edit my heap)
void testMinHeap() {
    std::cout << "\nMinHeap Tests!\n";

    minHeap<string> heap;

    heap.addNode("hello", 1);
    std::cout << "expected hello: " << heap.popMin().data << "\n";

    heap.addNode("first", 1);
    heap.addNode("second", 2);
    std::cout << "expected first: " << heap.popMin().data << "\n";
    std::cout << "expected second: " << heap.popMin().data << "\n";

    heap.addNode("low priority", 2);
    heap.addNode("high priority", 1);
    std::cout << "expected high priority: " << heap.popMin().data << "\n";

    heap.addNode("four", 4);
    heap.addNode("two", 2);
    heap.addNode("five", 5);
    heap.addNode("one", 1);
    std::cout << "expected one: " << heap.popMin().data << "\n";
    std::cout << "expected two: " << heap.popMin().data << "\n";

    heap.addNode("A", 1);
    heap.addNode("B", 1);
    heap.addNode("C", 0);
    std::cout << "expected C: " << heap.popMin().data << "\n";
    std::cout << "expected A: " << heap.popMin().data << "\n";

    heap.addNode("500", 0);
    heap.addNode("10", 5);
    std::cout << "expected 500: " << heap.popMin().data << "\n";
}



// yay! it finally mostly works! or at least it mostly works! it still throws a few errors 
// but frankly it works well enough for me to move forward


// so, it SHOULD be very easy to implement the priority queue from here: 
/*
 * ``MinPriorityQueue`` maintains a priority queue where the lower
 *  the priority value, the sooner the element will be removed from
 *  the queue.
 *  @param T the representation of the items in the queue
 * 
interface MinPriorityQueue<T> {

     * @return true if the queue is empty, false otherwise
    fun isEmpty(): Boolean

     * Add [elem] with at level [priority]
    fun addWithPriority(elem: T, priority: Double)

     * Get the next (highest priority) element and remove this element from the queue.
     * @return the next element in terms of priority.  If empty, return null.
    fun next(): T?

     * Adjust the priority of the given element
     * @param elem whose priority should change
     * @param newPriority the priority to use for the element
     *   the lower the priority the earlier the element int
     *   the order.
    fun adjustPriority(elem: T, newPriority: Double)
}
*/


/**
 * Maintains a priority queue where lower priority value means sooner removal.
 * @param queueType the representation of the items in the queue
 */
template <typename queueType>
class MinPriorityQueue{
    // okay so basically we just need to keep a MinHeap

    minHeap<queueType> MinHeapQueue;
    // we pass queuetype to minheap which passes to heapnode 
    // queue type is type of data, priority type is still double 

    public:
    /**
     * Checks if the queue is empty.
     * @return true if the queue is empty, false otherwise
     */
    // @return true if the queue is empty, false otherwise
    bool isEmpty(){
        return MinHeapQueue.isEmpty();
    }

    /**
     * Adds an element with the given priority.
     * @param data element to add
     * @param priority priority level, lower means sooner removal
     */
    // Add [elem] with at level [priority]
    void addWithPriority(queueType data, double priority){
        MinHeapQueue.addNode(data, priority);
    }

    /**
     * Gets the next highest-priority element and removes it.
     * @return the next element in terms of priority
     */
    // Get the next (highest priority) element and remove this element from the queue.
    // @return the next element in terms of priority.  If empty, return null.
    queueType next(){
        HeapNode minNode = MinHeapQueue.popMin();
        return minNode.data; // we have access to priority too if needed
    }

    /**
     * Adjusts the priority of the given element.
     * @param data element whose priority should change
     * @param newPriority the priority to use, lower means earlier in order
     */
    // Adjust the priority of the given element
    // @param elem whose priority should change
    // @param newPriority the priority to use for the element
    // the lower the priority the earlier the element int the order.
    void adjustPriority(queueType data, double newPriority){
        // okay this one is kinda tricky since we don't have a function for this 
        // SYKE! we can just add one 
        MinHeapQueue.adjustPriority(data, newPriority);
    }

};


/**
 * Tests MinPriorityQueue functionality.
 */
void testPriorityQueue(){
    std::cout << "\nPriority Queue Tests!\n";

    MinPriorityQueue<string> queue;

    std::cout << "expected true: " << queue.isEmpty() << "\n";

    queue.addWithPriority("low", 5);
    queue.addWithPriority("high", 1);
    queue.addWithPriority("medium", 3);

    std::cout << "expected high: " << queue.next() << "\n";
    std::cout << "expected medium: " << queue.next() << "\n";
    std::cout << "expected low: " << queue.next() << "\n";

    queue.addWithPriority("A", 5);
    queue.addWithPriority("B", 3);
    queue.addWithPriority("C", 1);
    queue.adjustPriority("A", 0);
    std::cout << "expected A: " << queue.next() << "\n";
    std::cout << "expected false: " << queue.isEmpty() << "\n";
}




// comment out since we import into Dijkstra
// void main() {
//     testMinHeap();
//     testPriorityQueue();
// }

// great! they both work! still throwing errors but whatever i'm moving on! 
// time to learn how to import from a different file! 
