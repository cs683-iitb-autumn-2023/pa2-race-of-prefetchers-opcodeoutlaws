## Task 1: IP Stride Prefetching
We found the code given as a part of the problem statement to be slightly incorrect. We have made a minor modification to the code and were able to achieve higher speedups. The function `prefetch_line` expects the base address of a cache block i.e. the bits `[0:5]` to be set to 0.

Here is the plot for all the traces:

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/48720143/078143f5-0222-4670-9339-3fdfaf324934">



## Task 2: Stream Prefetching
We implemented the approach suggested in the linked research paper http://hps.ece.utexas.edu/pub/srinath_hpca07.pdf to implement this.

Quick observations:
- Some trace are favorable for a stream prefetcher.
- If the aggressiveness is increased too much, we start noticing decrease in IPC.

Here is the plot for all traces

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

## Stream Stride

#1. Speedup

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142026329/a57904a3-6ebe-448c-911e-a44c325b3533">

#2. L1D MPKI

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142026329/212f9860-7f7e-49fc-b786-9f70529a0fd8">

#3. L2C MPKI

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142026329/bd6b41cf-9424-4069-85e2-8be85492d7e0">

#4. Prefetch Accuracy

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142026329/74c50d3f-c4ef-45d8-a189-9c59b1638356">





