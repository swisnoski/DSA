/* 
just reusing my heap from the last assignment to do heapsort 
see assignment three for more information
*/


#include <vector>
#include <iostream>
#include <set> 
#include <map>
#include <string>
using namespace std; 


template <typename heapDataType>
struct HeapNode {
    heapDataType data;
    double priority;
};

template <typename heapDataType>
class minHeap{
    private:
    std::vector<HeapNode<heapDataType>> MinHeapArray;
    // so instead of a vector of just doubles we can instead declare a vector of heap nodes 

    public:
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

    // okay so we need to go back and add in this function to actually make the priority queue work 
    bool isEmpty(){
        return (MinHeapArray.size() == 0);
    }

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
