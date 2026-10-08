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