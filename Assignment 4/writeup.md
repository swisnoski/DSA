# Assignment 4 - Sorting Algorithms Writeup 

Sam Wisnoski  
9/29/2026   
Prof. Paul Ruvolo 

## Different Algorithms Implemented

### 1. Insertion Sort

Given an unsorted list of elements, we go through the list one by one and insert it into a
new vector. When we insert it, we compare it to the element next to it. If it is bigger, we 
swap the elements and compare it to it's new neighbor. We continue this process until the 
newly added element is no longer bigger than the element to it's right. 

This, in the worst case, would take roughly 1 + 2 + 3 + 4... + n = (n^2 - n)/2
for a big-theta value of n^2

Expected time complexity: n^2, since each new element may have to be compared against everything already sorted.

### 2. Selection Sort

Given an unsorted list of elements, we go through the list and select the smallest one,
and move it to the front. Then we look through all elements not yet selected and do the same,
adding the next to the front. For n elements, we need to look through n + (n-1) + (n-2) + ... + 2 + 1.
This, in the worst case, would also have a big-theta value of n^2.

Expected time complexity: n^2, since we are always comparing against a large unsorted part.

### 3. Heap Sort

We already made a minheap from scratch last week, so I'm not going to cover it in a lot of detail here. 

Basically, we place a new element in the list, and bubble it up the heap until it does not break 
the heap invarient (the rule that parents are greater than or equal to thier children in a binary tree).
To remove an element, we pop the root, add the last node as the new root, and then bubble it down 
until it no longer breaks the heap invarient. 

Because bubbling up and bubbling down have a maximum number of operations of size log(n) because 
a heap has layers that double in size, and because to add/remove to a heap requires n operations, we 
have a time complexity of O(n log(n)).

Expected time complexity: Θ(n log n). We add n elements and pop n elements, and each add or pop only has to move up or down the heap's log(n) layers.

### 4. Merge Sort

We can recursively split the vector in half until we get down
to our base case, just comparing vectors to their neighbors. We 
then reassemble these vectors together by pulling elements off 
the front of the lists (since we know that they are sorted). Each time we 
do this, we split in half so we have log(n) levels of splitting,
and then we need to iterate through each element (n elements) for each
merge, so we end up with a rough computation time of nlog(n).

Expected time complexity: Θ(n log n). There are log(n) levels of splitting, and each level touches all n elements once while merging.

### 5. Radix Sort

Lastly, radix sort goes one digit by a time, from the ones, tens, hundreds, thousands, etc place
(if working in base ten). Each time we sort only by that one digit, so we have to sort n elements
a number of times equal to the number of digits in the longest number. So the time complexity for
this algorithm is O(n * digits). This is generally faster than n^2, but it also introduces a new element,
so it's more unpredictable than some other algorithms, in the odd case you were working with numbers with 
many many digits. 

Expected time complexity: Θ(n * d), where d is the number of digits in the largest number. 

## Testing Strategy

To benchmark the algorithms, I generated a file benchmarking.cpp, which imports all my sorting algorithms and has a small
random number generator to generate vectors of a different size. I orginally tested five list sizes: 10, 100, 1,000, 10,000, and 100,000, but then
decided to push the system a bit and run an additional test at size 200,000.

For each of the different sizes, we ran each algorithm ten times and averaged the result. This was only a slight mistake, as the selection sort 
runtime was around a total of 820 seconds for the final ten runs with size 200,000. 

## Results

Average runtime in seconds (10 trials each):

| Size | Insertion | Selection | Heap | Merge | Radix |
|---|---|---|---|---|---|
| 10 | 2.114e-05 | 4.5e-06 | 2.77e-05 | 0.00011074 | 9.095e-05 |
| 100 | 0.00015062 | 6.51e-05 | 0.00013953 | 0.00134876 | 0.00019859 |
| 1000 | 0.00147601 | 0.00312689 | 0.00097879 | 0.0133681 | 0.00060275 |
| 10000 | 0.0429875 | 0.25301 | 0.0096792 | 0.111315 | 0.00305993 |
| 100000 | 3.70905 | 23.7392 | 0.104515 | 1.18721 | 0.0236402 |
| 200000 | 12.7105 | 81.9865 | 0.16865 | 1.62503 | 0.0377663 |

## Analysis

For the smaller lists, selection sort is suprisingly the fastest, but sizes of 10 and 100 doesn't have very meaningful scalability. 
Interestingly, merge sort is initially the slowest, perhaps because of how it's allocating memory to many different vectors. 

As we scale up our lists, the differences in time complexities begin to become obvious. As we move to 10,000, 100,000, and 200,000, we see 
the runtime for insertion and selection sort spike signifigantly, while heap, merge, and radix sort all stay on the lower side. 

Although both insertion sort and selection sort have a time complexity of n^2, insertion sort beat selection by nearly 7x, perhaps because 
selection sort always compares against every value in the list, while insertion sort only compares up until it finds a "good spot" for it's 
insertion. 

Similarly, although heap and merge both have a calculated nlog(n) runtime, we see heap beat merge by nearly 10x. Once again, I 
will contribute this disparity to the creation of vectors. 

Overall, though, radix sort is clearly the best for longer lists. This is because in testing, we only generated numbers with up to three digits, 
so it's time complexity was akin to (n*3), which is essentially linear. 