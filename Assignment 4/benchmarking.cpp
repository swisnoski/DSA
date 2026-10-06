/* 
Sam Wisnoski 

Assignment 4 - Sorting Algorithms 

9/29/2026 

*/


/*
Now that we have all of our algorithms, we can do some benchmarking! 
*/

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <chrono>
#include <cstdlib>


// first, let's import all of our sorting functions from sorting_algos.cpp
#include "sorting_algos.cpp"

// we can just generate a list by using the random class 
// we just generate a vector, loop for a desired size, and 
// generate a random integer between 0 and 1000
std::vector<int> makeRandomList(int desiredSize) {
    std::vector<int> randomList;
    for (int index = 0; index < desiredSize; index++) {
        randomList.push_back(std::rand() % 1000);
    }
    return randomList;
}

// and then we can test 
int main() {
    // we will test five vectors of different sizes - 10, 100, 1000, 10000, and 100000
    std::vector<int> vectorSizes = {10, 100, 1000, 10000, 100000};

    std::cout << "size, insertion, selection, heap, merge, radix\n";

    // loop through the five different sizes 
    for (int size : vectorSizes) {
        double insertionTime = 0;
        double selectionTime = 0;
        double heapTime = 0;
        double mergeTime = 0;
        double radixTime = 0;


        // for each list, we will run through 10 times and then average 
        for (int index = 0; index < 10; index++) {
            // so we generate a random list of the proper size   
            std::vector<int> randomList = makeRandomList(size);

            // for each sort, we grab the time before and after it runs,
            // and add the difference (in seconds) to that sort's total
            // it looks kinda complex but we are really just doing the 
            // same thing over and over again
            auto startTime = std::chrono::high_resolution_clock::now();
            insertionSort(randomList);
            auto endTime = std::chrono::high_resolution_clock::now();
            insertionTime += std::chrono::duration<double>(endTime - startTime).count();

            startTime = std::chrono::high_resolution_clock::now();
            selectionSort(randomList);
            endTime = std::chrono::high_resolution_clock::now();
            selectionTime += std::chrono::duration<double>(endTime - startTime).count();

            startTime = std::chrono::high_resolution_clock::now();
            heapSort(randomList);
            endTime = std::chrono::high_resolution_clock::now();
            heapTime += std::chrono::duration<double>(endTime - startTime).count();

            startTime = std::chrono::high_resolution_clock::now();
            mergeSort(randomList);
            endTime = std::chrono::high_resolution_clock::now();
            mergeTime += std::chrono::duration<double>(endTime - startTime).count();

            startTime = std::chrono::high_resolution_clock::now();
            radixSort(randomList);
            endTime = std::chrono::high_resolution_clock::now();
            radixTime += std::chrono::duration<double>(endTime - startTime).count();
        }

        // and then we divide each time by 10 (because we performed 10 
        // trials), and print out our values!
        std::cout << size << ", "
                  << insertionTime / 10 << ", "
                  << selectionTime / 10 << ", "
                  << heapTime / 10 << ", "
                  << mergeTime / 10 << ", "
                  << radixTime / 10 << "\n";
    }

    return 0;
}