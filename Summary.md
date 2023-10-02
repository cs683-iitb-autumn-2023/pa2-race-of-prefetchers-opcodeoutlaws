## Task 1: IP Stride Prefetching
We found the code given as a part of the problem statement to be slightly incorrect. We have made a minor modification to the code and were able to achieve higher speedups. The function `prefetch_line` expects the base address of a cache block i.e. the bits `[0:5]` to be set to 0.

Here is the plot for all the traces:
![image](https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/48720143/078143f5-0222-4670-9339-3fdfaf324934)



## Task 2: Stream Prefetching
We implemented the approach suggested in the linked research paper http://hps.ece.utexas.edu/pub/srinath_hpca07.pdf to implement this.

Quick observations:
- Some trace are favorable for a stream prefetcher.
- If the aggressiveness is increased too much, we start noticing decrease in IPC.

Here is the plot for all traces:

#1 Speedup

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/48720143/ebe29c1b-e1ce-4cc6-8fcf-35e0bd582dfa">

#2 L1D MKPI

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/48720143/3f2a79a3-c9cb-40af-93c3-06f532d1dda4">

#3 L2C MKPI

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/48720143/7dd69571-5c4a-4b8d-824e-be647713bf10">

#4 Prefetch Accuracy

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/48720143/1a497965-dd07-4c25-bf96-3f945f3eef5a">





## Task 3: Prefetch Throttling
We have used the prefetch accuracy to decide how the prefetch aggresiveness is decided. There are 5 predefined configurations from least aggressive (conservative) to most aggressive prefetcher degree + distance combinations.

For stream prefetcher:

| Prefetch Accuracy | Degree, Distance |
| ----------------- | -----------------|
| < 0.2             | 1,4              |
| < 0.4             | 1,8              |
| < 0.6             | 2,16             |
| < 0.8             | 4,16             |
| < 1               | 4,64             |



For IP stride:
| Prefetch Accuracy | Degree |
|-------------------|--------|
| < 0.2             | 1      |
| < 0.4             | 2      |
| < 0.6             | 3      |
| < 0.8             | 4      |
| < 1               | 8      |



Hence we adjust the aggressiveness of the prefetcher based on the prefetch accuracy!
Here's the comparison of throttling based prefetcher with the normal prefethers of respective kinds:


## Bonus Task
This implements the mix of IP stride and Stream prefetcher. We have used the stream prefetcher here with a modification to detect strides. The strdies are prefetched such that they align with the pattern that is observed in the monitered region.

Here are the speedup observations:
