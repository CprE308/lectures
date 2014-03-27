% Lecture 30 - Page Replacement 
% CprE 308
% March 28, 2014

# Memory Management

## Overview
Ideal World (for the programmer):

 - I'm the only process in the world
 - I have a huge amount of memory at my disposal

Real World

 - Many processes in the system
 - Not enough memory for them all

## Goal of Memory Management
 - Present the ideal world view to the programmer, yet implement it on a real system

# Page Faults

## Page Fault
What happens if the required page is not in memory?

"Page-fault" trap is initiated, OS gets control

 1. Find a free page frame
 2. Read the desired page from disk into memory
 3. Modify the page tables
 4. Restart the interrupted instruction

## OS Issues
<!---
Write these three on the board.
-->
 - Fetch policy - when to fetch pages into memory?
 - Placement policy - where to place pages?
 - Replacement policy
 - All combined in the handling of a page fault

## A Simple Paging Scheme
Fetch policy

 - start process off with no pages in primary storage
 - bring in pages on demand (and only on demand)
    - this is known as demand paging

Placement policy

 - it doesn't matter - put the incoming page in the first available page frame

Replacement policy

 - replace the page that has been in primary storage the longest (FIFO policy)

## Improving the Fetch Policy
<!---
Idea: Bring in more pages than just used to hopefully trigger less page faults later.
-->
![](img/fetch_policy.png)

# Page Replacement

## Improving the Replacement Policy
 - When is replacement done?
    - doing it "on demand" causes excessive delays
    - should be performed as a separate, concurrent activity
 - Which pages are replaced?
    - FIFO policy is not good
    - want to replace those pages least likely to be referenced soon

## The "Pageout Daemon"
<!---
Daemon keeps track of in use pages.  If a page needs to be cleared, the pageout daemon will clear it.
If necessary (when modified=1), we will write the page to disk before clearing.
-->
![](img/pageout_daemon.png)

## Page Replacement

Problem Statement:

 > A page is being brought into memory which has no free space. Which page should we replace to make space?

## Page Replacement - Mental Exercise
<!---
Ideal: Remove page that will be used longest in future (or never again)
-->
> - What is the optimal policy, if we had the knowledge of the future page requests
> - Policy: Choose the page which will be referenced farthest in the future
> - However, we don't know the future
    - Hope that the next few references will be for pages that were recently referenced
> - What's the use of knowing about this policy?
    - Will help us access the performance of a real algorithm

## Choosing the Page to Remove
Policies:

 - FIFO (First-In-First-Out)
 - NRU (Not-Recently-Used)
 - Second Chance
 - LRU (Least-Recently-Used)
 - Clock Algorithm(s)
 - Working Set Algorithm

Two issues:

 - How good is the decision?
 - Overhead?
    - Cost per memory access - should be very small
    - Cost per replacement - can be larger

## FIFO
<!---
First in, first out.
-->
Example: 8 pages, 4 page frames

![](img/fifo.png)

Hit ratio: 16/33

## Help from Hardware
For each page frame:

 - Referenced Bit(R) - 1 if page frame has been referenced recently
 - Modified Bit(M) - 1 if page has been modified since it has been loaded
    - Also known as "dirty bit"

## Not Recently Used Algorithm (NRU)
Pages are classified into 4 classes:

 - Class 0: not referenced, not modified (R=0, M=0)
 - Class 1: not referenced, modified (R=0, M=1)
 - Class 2: referenced, not modified (R=1, M=0)
 - Class 3: referenced, modified (R=1, M=1)

NRU removes page at random from lowest number non empty class

The R bit is cleaned periodically (based on a timer)

## Second Chance
 - Based on FIFO
 - Old pages are inspected for replacement
    - But are given a "second chance" if they have been used recently

## Second Chance Algorithm
<!---
If all pages referenced, degenerates into pure FIFO
-->
![](img/second_chance.png)

 - Pages sorted in FIFO order (time of arrival)
 - If earliest page has R=1, then give it a second chance by moving it to the end of the list

## Clock Algorithm - Another Implementation of Second Chance
<!---
Exact same idea as second chance, but doesn't do a lot of inefficient moving of pages
-->
 - Order pages in circular list
 - "Hand" of the clock points to the page to be replaced currently
 - When required to evict a page
    - If page pointed to has R=0, then evict it
    - If R=1, then reset R and move hand forward
 - Clock algorithm can be used with NRU (decision based on both R and M bits)

## Least Recent Used (LRU)
 - Replace the page in memory which has been unused for the longest time
 - \structure{Locality of Reference:} pages used in the near past will be used in the near future
    - True in typical cases

## Least Recently Used (LRU)
Example: 8 pages, 4 page frames

![](img/lru.png)

## LRU Implementation
<!---
Can also work in hardware with special counter registers
-->
> - Think of how you would implement it

> - One possible implementation:
    - list of pages, most recently used at front, least at rear
    - update this list every memory reference
    - when required to evict a page, choose the one at the rear of the list
  
> - Way too expensive!

## Not Frequently Used (NFU)
<!---
Problem? Something used a lot at the beginning but not anymore will stick around too long.
-->
 - Requires a software counter associated with each page, initially zero
 - At each clock interrupt, OS scans all the pages in memory
 - For each page, the R bit is added to the counter
 - The page with the lowest counter is chosen

## Aging - Approximating LRU
<!---
Counters shifted right 1 bit before the R bit is added in.  Then R bit added to leftmost rather than rightmost. (Basically a shift register)
-->
![](img/aging.png)

## Example
![](img/example.png)

## Questions:
Which page frame will be replaced?

> - FIFO
    - PFN 3 since loaded longest ago at time 20
> - LRU
    - PFN 1 since referenced longest ago at time 160
> - Clock
    - Clear R in PFN 3 (oldest loaded), clear R in PFN 2 (next oldest loaded), victim PFN is 0 since R=0
