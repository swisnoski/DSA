/* 
Sam Wisnoski 

Assignment 5 - Divide and Conquer and Dynamic Programming

10/8/2026

*/

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <chrono>
#include <cstdlib>

/*
Time Strassen and regular matrix multiplication on various size problems.
You may find the Strassen does not reliably outperform conventional matrix multiplication. 
A hybrid approach may work better where you use conventional matrix multiplication for smaller 
matrices and Strassen for larger matrices. What appears to be the optimal size matrix where 
you should switch between Strassen and conventional multiplication?
*/


// first, let's import our Matrix class from matrix.cpp
#include "matrix.cpp"

// we can generate a random matrix to multiply, we will write a short function 
// that returns a random matrix of doubles given a specific size 
Matrix<double> makeRandomMatrix(int desiredSize) {
    // returns a desiredSize x desiredSize matrix of random values from 0-99

    // all we really do is just loop through every cell and set it to a random integer between 0 and 99
    Matrix<double> randomMatrix(desiredSize);
    for (int rowIndex = 0; rowIndex < desiredSize; rowIndex++) {
        for (int columnIndex = 0; columnIndex < desiredSize; columnIndex++) {
            randomMatrix.setCell(columnIndex, rowIndex, std::rand() % 100);
            // I guess this is actually ints but we are using doubles because they can store 
            // larger numbers 
        }
    }
    return randomMatrix;
}


// for this benchmarking, we are just making the assumption that our unit tests are correct
// and our matrix multiplication functions are correct. We can know this is doubly true because 
// I'm great at writing code and it simply must be right. 

// this is our first benchmark, comparing block, strassen, and simple across different sizes
void benchmarkSizes() {
    // times block, strassen, and simple across a range of matrix sizes
    std::vector<int> matrixSizes = {2, 4, 8, 16, 32, 64, 128, 256, 512};

    // we will run each of these five times and then average the results 
    int trials = 2;

    // loop through the different sizes
    for (int size : matrixSizes) {
        double multiplyTime = 0;
        double strassenTime = 0;
        double simpleTime = 0;
        bool allMatch = true;

        // run each time equal to number of trials 
        for (int index = 0; index < trials; index++) {
            Matrix<double> matrixA = makeRandomMatrix(size);
            Matrix<double> matrixB = makeRandomMatrix(size);

            // we use chrono to mark start and end times for both types of multiplication
            auto startTime = std::chrono::high_resolution_clock::now();
            Matrix<double> multiplyResult = matrixA.blockMultiply(matrixB);
            auto endTime = std::chrono::high_resolution_clock::now();
            multiplyTime += std::chrono::duration<double>(endTime - startTime).count();

            startTime = std::chrono::high_resolution_clock::now();
            Matrix<double> strassenResult = matrixA.strassenMultiply(matrixB);
            endTime = std::chrono::high_resolution_clock::now();
            strassenTime += std::chrono::duration<double>(endTime - startTime).count();

            startTime = std::chrono::high_resolution_clock::now();
            Matrix<double> simpleResult = matrixA.simpleMultiply(matrixB);
            endTime = std::chrono::high_resolution_clock::now();
            simpleTime += std::chrono::duration<double>(endTime - startTime).count();
        }

        // and then we divide each time by the number of trials
        std::cout << size << ", " << multiplyTime / trials << ", " << strassenTime / trials << ", " << simpleTime / trials << "\n";
    }
}

// and this is our second benchmark, where we keep the size fixed at and try out
// different thresholds for the hybrid, comparing it to simple each time
void benchmarkThresholds() {
    // times simple vs. hybrid on a fixed size matrix for different thresholds
    int size = 2048;
    std::vector<int> thresholds = {32, 64, 128, 256, 512, 1024, 2048};

    // two trials each, averaged
    int trials = 3;

    std::cout << "Threshold, simple, hybrid\n";

    for (int threshold : thresholds) {
        double simpleTime = 0;
        double hybridTime = 0;

        for (int index = 0; index < trials; index++) {
            Matrix<double> matrixA = makeRandomMatrix(size);
            Matrix<double> matrixB = makeRandomMatrix(size);

            // using hybrid multiply for consistency with the max threshold so it doesn't ever 
            // use strassen multiplication
            auto startTime = std::chrono::high_resolution_clock::now();
            Matrix<double> simpleResult = matrixA.hybridMultiply(matrixB, size);
            auto endTime = std::chrono::high_resolution_clock::now();
            simpleTime += std::chrono::duration<double>(endTime - startTime).count();

            startTime = std::chrono::high_resolution_clock::now();
            Matrix<double> hybridResult = matrixA.hybridMultiply(matrixB, threshold);
            endTime = std::chrono::high_resolution_clock::now();
            hybridTime += std::chrono::duration<double>(endTime - startTime).count();
        }

        std::cout << threshold << ", " << simpleTime / trials << ", " << hybridTime / trials << "\n";
    }
}

int main() {
    // runs whichever benchmark we want
    //benchmarkSizes();
    benchmarkThresholds();
    return 0;
}