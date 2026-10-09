# Benchmarking Results

All times below are recorded in seconds.

## Benchmarking Block Multiplication vs. Strassen Multiplication

In the beginning, I only implemented basic block multiplication and Strassen. I benchmarked them against each other by averaging three trials and found that Strassen was faster in all cases n >= 8, which was surprising to me, since I had read online that Strassen was typically only beneficial over normal multiplication with huge matrices. I only computed up to 128 since the matrices were taking so long to compute.

| Size | Block | Strassen |
|---|---|---|
| 2 | 0.00023498 | 0.00031702 |
| 4 | 0.00208744 | 0.00239884 |
| 8 | 0.0133977 | 0.012872 |
| 16 | 0.0845014 | 0.0783672 |
| 32 | 0.511863 | 0.4025 |
| 64 | 3.75076 | 2.75234 |
| 128 | 83.9095 | 50.7231 |

## Adding Simple Matrix Multiplication

So then, I went back and added simple multiplication that just iterates cell by cell and does simple matrix multiplication. This heavily outperformed Strassen, and even more heavily outperformed block. I switched to a release build instead of debug and only ran two trials to try to speed things up, but my Strassen still took over 300x longer than simple even at n=512. I figured it must be because Strassen was taking up way more memory and allocating so many different vectors. I have no idea why this was running so slow.

| Size | Block | Strassen | Simple |
|---|---|---|---|
| 2 | 1.64e-05 | 1.555e-05 | 2.5e-07 |
| 4 | 0.00018255 | 0.00015155 | 6e-07 |
| 8 | 0.00116085 | 0.00093605 | 1.95e-06 |
| 16 | 0.00745495 | 0.00607255 | 5.9e-06 |
| 32 | 0.0657516 | 0.0441146 | 3.01e-05 |
| 64 | 0.507257 | 0.316803 | 0.0002615 |
| 128 | 4.09658 | 2.24031 | 0.00203995 |
| 256 | 32.7651 | 16.1963 | 0.0187127 |
| 512 | 266.161 | 121.832 | 0.356119 |

## Hybrid mixing Strassen and regular multiplication

So, after looking more into this problem online (and also just following the instructions of the assignment), I figured the only chance I had at making something better than simple was some combination where we used Strassen on the larger matrices and simple multiplication on the smaller ones. As we can see above, although Strassen is way slower overall, simple multiplication scales more rapidly with size than Strassen does. From 256 -> 512, we see about 7x increase for Strassen compared to ~10x for simple.

For the hybrid, I used Strassen to split the matrix until it was smaller than or equal to the threshold, then switched to simple multiplication. I kept the size fixed at n = 2048 and averaged results across three trials. To keep the comparison fair, simple was timed by running the hybrid with the threshold set to the full matrix size (2048), so it never splits and both columns go through the same code path. You can see this worked in the last row, where simple and hybrid match.

| Threshold | Simple | Hybrid |
|---|---|---|
| 32 | 6.73832 | 10.2302 |
| 64 | 6.51767 | 6.46372 |
| 128 | 6.43768 | 5.24644 |
| 256 | 6.42573 | 4.73109 |
| 512 | 6.39169 | 4.80242 |
| 1024 | 6.45926 | 5.04243 |
| 2048 | 6.63681 | 6.65468 |

We already knew this, but small thresholds are terrible since Strassen doesn't perform well with lots of small matrices (though it's still way faster than doing Strassen all the way down). At a threshold of 64 the hybrid is about even with simple, and as we increase the threshold past that, the hybrid becomes faster than simple, with the best time at a threshold of 256 (about 26% faster than simple). Past 256 it starts to slow down again, since we're doing fewer Strassen splits. 2048 has the same score for each since a threshold of 2048 for a matrix of size 2048 is just doing simple multiplication all the way down. 

The best place to switch from Strassen to simple multiplication is around a matrix size of 256. Below 64 or so, Strassen is significantly slower than simple, and above 256 we lose some of the benefit of Strassen's splitting.
