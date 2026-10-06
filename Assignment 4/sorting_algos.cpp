/* 
Sam Wisnoski 

Assignment 4 - Sorting Algorithms 

9/29/2026 

*/


#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>


// ########### PART ONE: BASIC SORTING ALGORITHMS #################

/* 
Instructions: 
Implement at least four sorting algorithms. For each sorting algorithm you implement, provide an analysis of its 
computational complexity. At least one of your algorithms should be Θ(nlogn).

Here are some possible algorithms to implement: 
Heap sort
Radix sort
Insertion sort
Selection sort
Merge sort
Quick sort
*/


/* 
ALGORITHM 1: INSERTION SORT

This algorithm seemed super easy so we are going to try to tackle it first. 

Given an unsorted list of elements, we go through the list one by one and insert it into a 
new list (or vector, really). We can simply insert it by seeing if it is greater than the
element next to the right it, and if so, moving it to the right. 

This, in the worst case, would take roughly 1 + 2 + 3 + 4... + n = (n^2 - n)/2 
for a big-theta value of n^2
*/

std::vector<int> insertionSort(std::vector<int> vectorToSort){
    std::vector<int> sortedVector;
    for (int value : vectorToSort){
        if (sortedVector.empty()){
            sortedVector.push_back(value);
        }
        else{
            int index = 0;
            bool insertedForIndex = false;
             for (int value_compare : sortedVector){
                 if (value < value_compare){
                    sortedVector.insert(sortedVector.begin() + index, value);
                    insertedForIndex = true;
                    break;
                 }
                else{
                    index += 1;
                }
            }
            // if we don't make an insertion, it means the value is biggest, and we 
            // place it at the end of the list
            if(!insertedForIndex){
                sortedVector.push_back(value);
            }
        }

    }
    return sortedVector;
}


/* 
ALGORITHM 2: SELECTION SORT

This algorithm ALSO seemed super easy it's up next.  

Given an unsorted list of elements, we go through the list and select the smallest one, 
and move it to the front. Then we look through all elements not selected and do the same, 
adding the next to the front. For n elements, we need to look through n + (n-1) + (n-2) + ... + 2 + 1. 
This, in the worst case, would also have a big-theta value of n^2.
*/

std::vector<int> selectionSort(std::vector<int> vectorToSort) {
    for (int index = 0; index < vectorToSort.size(); index++) { // iterate a number of times equal to the size of the list 
        int lowest = vectorToSort[index]; // set low equal to start of next list 
        int lowestIndex = index; // keep track of the index of lowest 

        for (int newIndex = index + 1; newIndex < vectorToSort.size(); newIndex++) { // search the unsorted part of the list
            if (vectorToSort[newIndex] < lowest) { // check if any of the numbers are lower than the first 
                lowest = vectorToSort[newIndex]; // if so, update lowest 
                lowestIndex = newIndex; // and lowest index 
            }
        }

        // and then using swap, we put the smallest value at the current position
        std::swap(vectorToSort[index], vectorToSort[lowestIndex]);
    }
    return vectorToSort;
}


/* 
ALGORITHM 3: HEAP SORT

We already made a minheap from scratch last week, so this should be super simple. 

We know that a heap has a time of O(n log(n)) because of how a heap is structured in 
layers, with each layer doubling in size. 

*/
#include "heap.cpp"

std::vector<int> heapSort(std::vector<int> vectorToSort) {
    minHeap<int> helpingHeap;
    // for this heap we want to assign each value in the list a UNIQUE node with a name,
    // so we will just use the index as the data name, and value as the priority of the data 
    for (int index = 0; index < vectorToSort.size(); index++) {
        helpingHeap.addNode(index, vectorToSort[index]);
    }

    std::vector<int> sortedVector;
    // just by doing this we have already sorted all our data
    // Now we just need to pop out each of the elements in order 
    while (!helpingHeap.isEmpty()){
        sortedVector.push_back(helpingHeap.popMin().priority);
    }

    return sortedVector;
}


/* 
ALGORITHM 4: MERGE SORT

We can recursively split the vector in half until we get down 
to our base case, just comparing vectors to their neighbors. 

Each time we merge we just pull elements off the front of the two arrays 
into a larger array. We split in half so we have log(n) levels of splitting, 
and then we need to iterate through each element (n elements) for each 
merge, so we end up with a rough computation time of nlog(n). 
*/

std::vector<int> mergeSort(std::vector<int> vectorToSort) {
    // okay so this is a recursive algorithm so as always, we need to define our base case. 
    if (vectorToSort.size() <= 1) {
        return vectorToSort;
    }

    // we need to break our vectors down into two evenly split vectors,
    // we can just do this via indexing 
    std::vector<int> vectorOne(vectorToSort.begin(), vectorToSort.begin() + vectorToSort.size() / 2);
    std::vector<int> vectorTwo(vectorToSort.begin() + vectorToSort.size() / 2, vectorToSort.end());

    // and then some good old recursion! 
    // so this will sort each of the vectors by continuously breaking them down. 
    vectorOne = mergeSort(vectorOne);
    vectorTwo = mergeSort(vectorTwo);

    // once we actually start getting returns from the vectors, we need to figure out how to merge 
    // two smaller vectors into one larger vector. 

    std::vector<int> mergedVector;
   
    // we can commonly use two indexes to iterate through each of our vectors 
    int indexOne = 0;
    int indexTwo = 0;

    // we need to make sure that each of our indexes grow out of scope 
    while (indexOne < vectorOne.size() && indexTwo < vectorTwo.size()) {
        // we just compare the size of the value where the index is,
        // and iterate the index of the smaller value 
        if (vectorOne[indexOne] <= vectorTwo[indexTwo]) {
            mergedVector.push_back(vectorOne[indexOne]);
            indexOne++;
        }
        else {
            mergedVector.push_back(vectorTwo[indexTwo]);
            indexTwo++;
        }
    }

    // then, if one index runs our first, we add the rest of the remaining vector 
    // (which is already sorted )
    while (indexOne < vectorOne.size()) {
        mergedVector.push_back(vectorOne[indexOne]);
        indexOne++;
    }
    while (indexTwo < vectorTwo.size()) {
        mergedVector.push_back(vectorTwo[indexTwo]);
        indexTwo++;
    }

    // and lastly, we return the merged vector 
    return mergedVector;
}

// that one was kinda tough. I'll do one more and then call it a day. 



/* 
ALGORITHM 5: RADIX SORT

radix sort goes one digit by a time, from the ones, tens, hundreds, thousands, etc place 
(if working in base ten). Each time we sort only by that one digit, so we have to sort n elements 
a number of times equal to the number of digits in the longest number. So the time complexity for 
this algorithm is O(n * digits). This is generally faster than n^2, but it also introduces a new element,
so it's more unpredictable than some other algorithms. 
*/

std::vector<int> radixSort(std::vector<int> vectorToSort) {
    // nothing to sort in an empty vector
    if (vectorToSort.empty()) {
        return vectorToSort;
    }

    // the first thing we need to do is find the largest number 
    // so we know the value of digits, so we know how many numbers 
    // we need to process. 

    // we can do this simply by going through each element and comparing 
    // it to the largest.  
    int largest = vectorToSort[0];

    for (int value : vectorToSort) {
        if (value > largest) {
            largest = value;
        }
    }

    // so now we have the largest integer! 

    // placeValue is the place we're looking at (1, 10, 100, ...), and we keep going
    // as long as the largest number still has a digit in that place

    // and apparently this is how you iterate by 10s instead of ones 
    // so you can just multiply by 10 instead of adding one 
    for (int placeValue = 1; largest / placeValue > 0; placeValue *= 10) {

        // for each integer, we just want to go through the list and sort by that integer. 
        // we can do this by doing floor division and looking at the remainder 
        std::vector<std::vector<int>> numberCategories(10);

        // so we start by dividing by one and looking a
        for (int value : vectorToSort) {
            int digit = (value / placeValue) % 10;
            numberCategories[digit].push_back(value);
        }

        // and then we simply go through and put all the categories back into their 
        // vector, now sorted by that digit !
        int index = 0;

        for (int category = 0; category < 10; category++) {
            for (int value : numberCategories[category]) {
                vectorToSort[index] = value;
                index++;
            }
        }
    }

    return vectorToSort;
}



/* 
TESTING TIME!! 
*/

void printSortedVector(std::string name, std::vector<int> vectorToPrint) {
    std::cout << name << ": ";
    for (int value : vectorToPrint) {
        std::cout << value << " ";
    }
    std::cout << "\n";
}

void testSortingAlgorithms() {

    // ----------------------------------------------------
    // Test 1: Basic Unsorted Vector
    // ----------------------------------------------------
    {
        std::vector<int> numbers = {5, 2, 8, 1, 3};
        std::cout << "\nExpected: 1 2 3 5 8\n";

        printSortedVector("Insertion Sort", insertionSort(numbers));
        printSortedVector("Selection Sort", selectionSort(numbers));
        printSortedVector("Heap Sort", heapSort(numbers));
        printSortedVector("Merge Sort", mergeSort(numbers));
        printSortedVector("Radix Sort", radixSort(numbers));
    }

    // ----------------------------------------------------
    // Test 2: Reverse Sorted Vector
    // ----------------------------------------------------
    {
        std::vector<int> numbers = {9, 7, 5, 3, 1};
        std::cout << "\nExpected: 1 3 5 7 9\n";

        printSortedVector("Insertion Sort", insertionSort(numbers));
        printSortedVector("Selection Sort", selectionSort(numbers));
        printSortedVector("Heap Sort", heapSort(numbers));
        printSortedVector("Merge Sort", mergeSort(numbers));
        printSortedVector("Radix Sort", radixSort(numbers));
    }

    // ----------------------------------------------------
    // Test 3: Vector with Duplicate Values
    // ----------------------------------------------------
    {
        std::vector<int> numbers = {4, 2, 4, 1, 2};
        std::cout << "\nExpected: 1 2 2 4 4\n";

        printSortedVector("Insertion Sort", insertionSort(numbers));
        printSortedVector("Selection Sort", selectionSort(numbers));
        printSortedVector("Heap Sort", heapSort(numbers));
        printSortedVector("Merge Sort", mergeSort(numbers));
        printSortedVector("Radix Sort", radixSort(numbers));
    }

    // ----------------------------------------------------
    // Test 4: Empty Vector
    // ----------------------------------------------------
    {
        std::vector<int> numbers = {};
        std::cout << "\nExpected: (nothing)\n";

        printSortedVector("Insertion Sort", insertionSort(numbers));
        printSortedVector("Selection Sort", selectionSort(numbers));
        printSortedVector("Heap Sort", heapSort(numbers));
        printSortedVector("Merge Sort", mergeSort(numbers));
        printSortedVector("Radix Sort", radixSort(numbers));
    }

    // ----------------------------------------------------
    // Test 5: Single Element Vector
    // ----------------------------------------------------
    {
        std::vector<int> numbers = {7};
        std::cout << "\nExpected: 7\n";

        printSortedVector("Insertion Sort", insertionSort(numbers));
        printSortedVector("Selection Sort", selectionSort(numbers));
        printSortedVector("Heap Sort", heapSort(numbers));
        printSortedVector("Merge Sort", mergeSort(numbers));
        printSortedVector("Radix Sort", radixSort(numbers));
    }
}


// void main(){
//     std::cout << "main!\n";
//     testSortingAlgorithms();
// }