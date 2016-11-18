/*
Basics of program needed to complete assignment 7
*/

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TLB_SIZE 16
#define PAGES 256
#define PAGE_MASK 255

#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255

#define MEMORY_SIZE PAGES * PAGE_SIZE

// Max number of characters per line of virtual address file to be read (addresses.txt)
#define BUFFER_SIZE 10

typedef struct {
  int8_t pageFrame[256];
}MemoryPage;

struct tlbentry {
    uint8_t logical;
    uint8_t physical;
};

// TLB is kept track of as a circular array, with the oldest element being overwritten once 
//the TLB is full.
struct tlbentry tlb[TLB_SIZE];

//number of inserts into TLB that have been completed. Use as tlbindex % TLB_SIZE for 
//the index of the next TLB line to use.
int tlbindex = 0;

// pagetable[logical_page] is the physical page number for logical page. Value is -1 
// if that logical page isn't yet in the table.
int pagetable[PAGES];

//simulation of RAM
int8_t main_memory[MEMORY_SIZE];

// Pointer to memory mapped secondary storage 
int8_t *backing;

/**

*/

int main(int argc, const char *argv[])
{


  return 0;
}
