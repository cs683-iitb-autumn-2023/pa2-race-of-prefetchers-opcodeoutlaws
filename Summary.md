## Task 1: IP Stride Prefetching
We found the code given as a part of the problem statement to be slightly incorrect. We have made a minor modification to the code and were able to achieve higher speedups. The function `prefetch_line` expects the base address of a cache block i.e. the bits `[0:5]` to be set to 0. We got this information by reading this: https://github.com/0xd3ba/build-your-own-prefetcher/tree/main/1_prefetching_api

Here is the plot for all the traces:

Speed Up Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/12698f11-ef30-4590-9c76-2ac0c32a29bd">
  
L1D MPKI Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/ae7271e8-d605-4eec-81dd-3863c7d1a9bd">

L2C MPKI Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/b9a63f84-a28b-4db7-9f6a-18836452fec4">

Prefetch Accuracy Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/f1205199-c49f-4b53-aa52-889d139ed587">


## Task 2: Stream Prefetching
We implemented the approach suggested in the linked research paper http://hps.ece.utexas.edu/pub/srinath_hpca07.pdf to implement this.

Note:
- The stream prefetcher implemented for the below observations is in accordance to the above linked research paper. Implementing a simpler prefetcher with just three states and without the condition of monitering region yeilds us a better performance with approximately 20% jump for some of the traces especially gcc trace.

Here is the plot for all traces:


Speed Up Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/408d8991-b149-4603-9df0-a80b49937bab">


L1D MKPI Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/48720143/3f2a79a3-c9cb-40af-93c3-06f532d1dda4">

L2C MKPI Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/48720143/7dd69571-5c4a-4b8d-824e-be647713bf10">

Prefetch Accuracy Plot

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

Speed Up Plot
 
<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/0f280536-657f-471d-b501-0c8423aed025">

L1D MPKI Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/48720143/3f2a79a3-c9cb-40af-93c3-06f532d1dda4">

L2C MPKI Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/d3e69000-fd97-4e23-a82e-08676dff15eb">

Prefetch Accuracy Plot

<img width="500" alt="image" src="https://github.com/cs683-iitb-autumn-2023/pa2-race-of-prefetchers-opcodeoutlaws/assets/142027995/227bb00b-f2d2-49d7-a3be-ce9ef04feb61">






