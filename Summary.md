## Task 1: IP Stride Prefetching
We found the code given as a part of the problem statement to be slightly incorrect. We have made a minor modification to the code and were able to achieve higher speedups. The function `prefetch_line` expects the base address of a cache block i.e. the bits `[0:5]` to be set to 0.

Here is the plot for all the traces:

Speed Up Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/41dc6baf-cfb8-4580-b7da-77c4231fef64">

  
L1D MPKI Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/ae7271e8-d605-4eec-81dd-3863c7d1a9bd">


L2C MPKI Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/b9a63f84-a28b-4db7-9f6a-18836452fec4">


Prefetch Accuracy Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/f1205199-c49f-4b53-aa52-889d139ed587">


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

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/211f4e9c-2ff3-4f8c-916e-5a25a32d89a5">



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





