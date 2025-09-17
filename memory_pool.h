// Data Structure that is similar to stack allocator, but pre-divides memory into same-
// sized blocks and hands them out one at a time.
// Example real-world use case: Network Packets
// Mentality: "I create objects that die at different times."
// Implemented using a linked list
#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stdint.h>
#include <stdbool.h> 
#include <stddef.h>
using namespace std;

typedef struct {
    uint8_t *memory;    // Pointer to actual memory block
    size_t total_size;  // Total size of the memory pool
    size_t block_size;  // Size of each individual block
    void *free_list;    // Linked list pointing to each individual block
} memory_pool_t;

// Function Declarations:
static inline void mp_init(memory_pool_t *mp, uint8_t *memory, size_t total_size, size_t block_size);
// Return type below is a void pointer -- a generic pointer that can point to any type of 
// data. This is useful because now the memory does not care what you store in it:
static inline void* mp_alloc(memory_pool_t *mp);
static inline void mp_free(memory_pool_t *mp, void *block);

// Function Implementations:
static inline void mp_init(memory_pool_t *mp, uint8_t *memory, size_t total_size, size_t block_size) {
    mp->memory = memory;
    mp->total_size = total_size;
    mp->block_size = block_size;

    size_t num_blocks = total_size / block_size;
    mp->free_list = memory;  // Free list starts at the first block
    for (size_t i = 0; i < num_blocks - 1; i++) {
        uint8_t *current_block = memory + (i * block_size);
        uint8_t *next_block = memory + ((i + 1) * block_size);

        // Casting current_block pointer to "pointer to void pointer," then dereferencing
        // to access what's stored there. 
        *(void **)current_block = next_block;
    }
    uint8_t *last_block = memory + ((num_blocks - 1) * block_size);
    *(void **)last_block = NULL;
}
// Raw Memory handed out as pointer, regardless of type of data stored there.
// Point of function is to provide a block of memory to use. 
static inline void* mp_alloc(memory_pool_t *mp) {
    // Check to see if any blocks are available
    if (mp->free_list == NULL) {
        return NULL;  // No free blocks left
    }

    // Get first free block
    void *block = mp->free_list;
    // Update free list to point to the next free block using the pointer
    mp->free_list = *(void **)mp->free_list;
    return block;
}
// Function returns a block back to the pool in order to be used for future allocations
static inline void mp_free(memory_pool_t *mp, void *block) {
    // Returned block is put at front of the list
    *(void **)block = mp->free_list; // Block now points to old first free block

    // free_list is updated to point to the newly freed block
    mp->free_list = block; // block now replaces the first in the free list
}

#endif