% Lecture 36 -  File Systems Implementation
% CprE 308
% April 7, 2014

# Intro

## Todays topics: File System Implementation

 - Data Structures for files
 - Unix files, Inodes

# Blocks

## Unix File System - Inodes

### 

####

 - data structure on disk
 - one inode per file

####
![](img/inodes.png)

## Problem
![](img/blocks.png)

 - Disk = (long) sequence of block
 - Keep track of the blocks associated with a file

## Contiguous Allocation
<!---
Here, files are stored in blocks.  One file might have 7 blocks.

Imagine having to specify file size when starting a document.
-->
###

####

 - All disk blocks of a file allocated sequentially
 - Advantages
    - (very) Fast read
    - Useful for read-only file systems (CD-ROM)
    - Keeping track of blocks of a file is easy
 - Problems
    - Fragmentation with deletes
    - File growth might be expensive

####
![](img/contig_alloc.png)

## Linked List of Blocks
<!---
Idea: Store pointer to next block at beginning of block
Good or bad idea?

Easy to read sequentially, simply continue through.
Bad for random access.  To read block n, need to read all n-1 previous blocks.

Can use every disk block, directory entry only needs location of first block.
Bad because part of disk block is used for pointer.
-->
![](img/linked_list.png)

\pause

Sequential access is fast, random access is slow

## File Allocation Tables (FAT)
<!---
Can use entire block again (since pointer is in memory).
Still have to follow path for random access, but memory accesses are fast enough to not be a huge problem.

Con: needs lots of space.  200GB disk with 1-KB block size has 200M entries/blocks. With 4 byte entries, 800MB of memory for the FAT.  So, does not scale well to large disks.
-->
###

####

 - One entry per physical disk block;
 - FAT can be in main memory

####
![](img/fat.png)

## Disk Map in Unix
<!---
After attributes, first few entries are actual address (blue)
After a while, use single indirect block (green) that contains additional addresses.
Then, double indirect block (red) points to a bunch of single indirect blocks, then address.
Finally, if necessary, triple indirect block (grey).

In modern unix, i-node has first 12 disk block addresses of file.
Single indirect block, if block is 1KB and address is 4 bytes, will have 256 addresses. Files up to 268 KB.
Double indirect can have 10 + 2^16 blocks.

Up to 64 TB with triple
-->
![](img/disk_map.png)

## Optimization for Sparse Files
<!---
How to deal with it?
Could have disk addresses in inode point to nothing, then the OS could fill them in when reading.

Must be careful when copying, not all programs do this smartly.
-->
###
####
 - Suppose a file was large, but mostly zeros
 - Could be produced using lseek and write

\pause

####
![](img/opt.png)

## Additional Enhancements
 - Performance depends on: How many disk access are needed to read a file?
 - Store some data in the inode itself
    - Perhaps the whole file will fit in!
    - Need only 1 disk access for a small file
 - Increase block size

# Directories

## File Systems
 - Data Structures for Directories
 - Shared Files

## Unix Directory (V7)
Directories are files whose data is a list of filenames & inodes

![](img/dir.png)

Max filename size = 14 chars

## The UNIX V7 File System
![](img/v7.png)

The steps in looking up `/usr/ast/mbox`
