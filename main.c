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

typedef struct {
  uint16_t* addresses;
  uint32_t size;
}MemoryAddresses;

struct tlbentry {
    uint8_t logical;
    uint8_t physical;
};

// TLB is kept track of as a circular array, with the oldest element being overwritten once
//the TLB is full.
struct tlbentry tlb[TLB_SIZE];

//number of inserts into TLB that have been completed. Use as tlbindex % TLB_SIZE for
//the index of the next TLB line to use.
int32_t tlbindex = 0;

// pagetable[logical_page] is the physical page number for logical page. Value is -1
// if that logical page isn't yet in the table.
int pagetable[PAGES] = { [ 0 ... PAGES-1 ] = -1};

//simulation of RAM
int8_t main_memory[MEMORY_SIZE];

// Pointer to memory mapped secondary storage
int8_t *backing;

/**
Takes in logical page and returns physical page.
Return of -1 indicates that page is in secondary storage, not in RAM.
*/
int get_physical_page(int logical_page)
{
  // First check in TLB.
  
  // Next check page table.
  if (pagetable[logical_page] != -1)
    return pagetable[logical_page];

  // -1 means page is not in RAM, so copy over from secondary storage.
  memcpy(main_memory + logical_page * PAGE_SIZE, 
           backing + logical_page * PAGE_SIZE, PAGE_SIZE);
 
  // update page table
  pagetable[logical_page] = logical_page;

  return logical_page;
  
}

/**
Takes in logical address and retreives and prints value.
*/
void get_value_at(int logical_address)
{
  // First bit shift and mask to get offset and logical page.
  int offset = logical_address & OFFSET_MASK;
  int logical_page = (logical_address >> OFFSET_BITS) & PAGE_MASK;
  int physical_page = get_physical_page(logical_page);

  // Extract value and new physical address
  int physical_address = (physical_page << OFFSET_BITS) | offset;
  int8_t value = main_memory[physical_page * PAGE_SIZE + offset];            
  printf("Virtual address: %d Physical address: %d Value: %d\n", 
            logical_address, physical_address, value);
}

size_t count_lines(char* file_name){
  FILE* file_pointer;
  char* line = NULL;
  size_t len = 0;
  size_t line_count = 0;
  ssize_t read;

  file_pointer = fopen(file_name, "r");
  if(file_pointer == NULL){
    exit(-1);
  }

  while((read = getline(&line, &len, file_pointer)) != -1){
    line_count++;
  }

  fclose(file_pointer);
  free(line);

  return line_count;
}

MemoryAddresses* get_memory_addresses(){
  FILE* file_pointer;
  char* line = NULL;
  size_t len = 0;
  size_t count = count_lines("addresses.txt");
  ssize_t read;

  MemoryAddresses* output = calloc(1, sizeof(MemoryAddresses));
  output->size = count;
  output->addresses = calloc(count, sizeof(uint16_t));


  file_pointer = fopen("addresses.txt", "r");
  if(file_pointer == NULL){
    exit(-1);
  }

  count = 0;
  while((read = getline(&line, &len, file_pointer)) != -1){
    output->addresses[count] = (uint16_t) atoi(line);
    count++;
  }

  fclose(file_pointer);
  free(line);

  return output;
}

int main(int argc, const char *argv[])
{
  const char *backing_filename = argv[1]; 
  int backing_fd = open(backing_filename, O_RDONLY);
  //after the next instruction the secondary storage file can be viewed as an array
  backing = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, backing_fd, 0); 

  MemoryAddresses* addresses = get_memory_addresses();
  for(size_t i = 0; i < addresses->size; i++){
    get_value_at(addresses->addresses[i]);
  }

  return 0;
}
