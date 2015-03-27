% Lecture 30 - Paging
% CprE 308
% March 27, 2015

# Paging

## Review: Scenario
Ideal World (for the programmer)

 - I'm the only process in the world
 - I have more memory than I need at my disposal

Real World

 - Many processes in the system
 - Not enough memory for them all
 - Not all processes play nicely

## Review: Goal of Memory Management
 - Present the ideal world view to the programmer, yet implement it on a real system
 - Add memory protections without getting in the way of the programmer

## Review: Virtual Memory
<!--
Source: http://www.tldp.org/LDP/tlk/mm/memory.html

"To make this translation easier, virtual and physical memory are divided into handy sized chunks called pages. These pages are all the same size, they need not be but if they were not, the system would be very hard to administer. Linux on Alpha AXP systems uses 8 Kbyte pages and on Intel x86 systems it uses 4 Kbyte pages. Each of these pages is given a unique number; the page frame number (PFN)."
-->
![Abstract Memory Model](img/abstract_memory_model.gif)

## Structuring Virtual Memory
<!--
In paging the address space is divided into a sequence of fixed size units called page frames. A logical (virtual) address takes the form of a tuple <page, offset into page frame>.

In segmentation, the address space is divided into a preset number of segments like data segment (read/write), code segment (read-only), stack (read/write) etc. The logical components of programs are divided into these segments accordingly and the OS kernel enforces the security restrictions of each segment. A logical (virtual) address is represented as tuple <segment, offset into segment>.

Paging helps reduce fragmentation and ensures better memory management, while segmentation offers better security protections (ex: data fields can't be executed).  Most modern OS's employ a mixture of the two schemes.  Instead of an actual memory location the segment information includes the address of a page table for the segment. When a program references a memory location the offset is translated to a memory address using the page table. A segment can be extended simply by allocating another memory page and adding it to the segment's page table.
-->
 - Paging
    - Divides the address space into fixed-sized pages
    - Reduces fragmentation, increases efficiency
 - Segmentation
    - Divides the address space into variable-sized segments
    - Enables memory protections (Example: data, code, uninitialized, shared memory, etc.)
 - Modern OS's use a mixture of both schemes (paged segmentation)
 
## Typical Page Table Entry
<!--
Zooming in on a single entry:
 - Page frame number: physical location of page frame (then add offset)
 - Present/absent: 1 if entry is valid, 0 if not in memory
 - Protection: what sorts of accesses permitted. 0 for read/write, 1 for read only.  Or 3 bits: read,write,execute
 - Modified: Has it been modified?  *Dirty* bit
   "If the page has been modified, the operating system must preserve the contents of that page so that it can be accessed at a later time. This type of page is known as a dirty page and when it is removed from memory it is saved in a special sort of file called the swap file. Accesses to the swap file are very long relative to the speed of the processor and physical memory and the operating system must juggle the need to write pages to disk with the need to retain them in memory to be used again." Source: http://www.tldp.org/LDP/tlk/mm/memory.html
 - Referenced: set when referenced. Helps to choose eviction
 - Cache disabling: for pages that map device registers rather than memory.
-->
![Page Table Entry Fields](img/pt_entry.png)

## Paging Example
- Consider a virtual memory system with two processes
    - Let the physical memory consist of 24 words and the page frame size of four words 
    - Process 1 consists of 16 words (a through p)
    - Process 2 consists of 12 words (A through L)
    
## Paging Example (Process 1 Virtual Memory)
![Process 1 Virtual Memory](img/process1_1.png)

## Paging Example (Process 1 Virtual Memory)
![Process 1 Virtual Memory](img/process1_2.png)

## Paging Example (Process 2 Virtual Memory)
![Process 2 Virtual Memory](img/process2_1.png)

## Paging Example (Process 2 Virtual Memory)
![Process 2 Virtual Memory](img/process2_2.png)

## Paging Example (Physical Memory)
![Physical Memory - What are the contents of page 0?](img/physical_memory_1.png)

## Paging Example (Physical Memory)
![Physical Memory - What are the contents of page 1?](img/physical_memory_2.png)

## Paging Example (Physical Memory)
![Physical Memory - What are the contents of page 2?](img/physical_memory_3.png)

## Paging Example (Physical Memory)
![Physical Memory - What are the contents of page 3?](img/physical_memory_4.png)

## Paging Example (Physical Memory)
![Physical Memory - What are the contents of page 4?](img/physical_memory_5.png)

## Paging Example (Physical Memory)
![Physical Memory - What are the contents of page 5?](img/physical_memory_6.png)

## Paging Example (Physical Memory)
![Physical Memory](img/physical_memory_7.png)

## Paging Example (Physical Memory)
- Suppose process 1 is running and it tries to access the contents of the virtual address **15**, what is the result?

## Paging Example (Physical Memory)
![Physical Memory](img/physical_memory_7.png)

## Paging Example (Page Faults)
- Suppose process 1 is running and it tries to access the contents of the virtual address **15**, what is the result?
    - Virtual address **15** is in process 1's virtual page **3**.  According to the page table for process 1, the virtual page **3** is paged in physical memory as page **4**, which means the value **p** will be immediately fetched from memory.

## Paging Example (Page Faults)
- Suppose process 1 is running and it tries to access the contents of the virtual address **9**, what is the result?

## Paging Example (Page Faults)
![Physical Memory](img/physical_memory_7.png)


## Paging Example (Page Faults)
- Suppose process 1 is running and it tries to access the contents of the virtual address **9**, what is the result?
    - Virtual address **9** is in process 1's virtual page **2**.  According to the page table for process 1, virtual page **2** is not paged in physical memory (flagged as invalid in the page table).  A **page fault** occurs, and physical memory will need to be swapped before the value **j** can be fetched from memory.
    
## Paging Example (Address Translation)

Process 1

- Virtual Address **2** to Physical Address
- Physical Address **5** to Virtual Address

Process 2

- Virtual Address **2** to Physical Address
- Physical Address **22** to Virtual Address

## Paging Example (Address Translation)
![Physical Memory](img/physical_memory_7.png)

## Paging Example (Address Translation)

Process 1

- Virtual Address **2** to Physical Address
    - **10**
- Physical Address **5** to Virtual Address
    - **5**

Process 2

- Virtual Address **2** to Physical Address
    - **14**
- Physical Address **22** to Virtual Address
    - **10**

## Implementation Notes
- Virtual memory is just a concept
    - It's addresses/values are always contiguous
    - It's values only really exist in physical memory
    - Page frames are just logical groupings (that can be calculated on the fly)
- Only need to store page tables

## Implementation Notes
![Implementation Overview](img/implementation_1.png)

## Implementation Notes
- Virtual page frames are always in order starting at 0
    - No need to store virtual page numbers in page table (just store physical page numbers in order)
- Techinically we don't "store" addresses either

## Implementation Notes
![Implementation Overview](img/implementation_2.png)

## Implementation Notes
<!--
 2 bits are required to store 4 virtual pages indexes
 2 bits are required to store 4 possible offsets in each page
-->
- If our page table stores 4 virtual pages mappings how many bits do we need to represent each page?
- If our page size is 4 words, how many bits do we need to represent each possible page offset?

## Implementation Notes
![Address Translation](img/address_translation.png)

## Quiz
- Any questions before the quiz?

