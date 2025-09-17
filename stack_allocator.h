// Data structure with LIFO strategy (Stack Allocator) -> Last thing allocated is the
// first thing that is freed.
// Memory Manager Data Structure --> Focus on how memory space is divided and handed out
// Replacement for malloc which ought to be avoided in embedded systems (We need 
// predictable memory management without variable timing and memory fragmentation).
// Mentality: "I need workspace now, throw it all away when done."
#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include <stdint.h>
#include <stdbool.h> 
#include <stddef.h>
using namespace std;

typedef struct {
    uint8_t *memory;
    size_t size;  // Size of entire buffer/memory. 
    size_t top; 
} stack_allocator_t;

// Function Declarations:
static inline void stack_init(stack_allocator_t *sa, uint8_t *memory, size_t size);
static inline void* stack_alloc(stack_allocator_t *sa, size_t bytes);
static inline void stack_free_to(stack_allocator_t *sa, void *ptr);  // Free everything above this point
static inline void stack_reset(stack_allocator_t *sa);               // Free everything

// Function Implementations:
static inline void stack_init(stack_allocator_t *sa, uint8_t *memory, size_t size) {
    sa->memory = memory;
    sa->size = size;
    sa->top = 0;
}
// stack_alloc provides chunk of memory to use --> return type is generic ptr of any type.
// fxn uses memory that YOU have provided, so arbitrary memory location from malloc is 
// avoided. 
static inline void* stack_alloc(stack_allocator_t *sa, size_t bytes) {
    if (sa->top + bytes > sa->size) {
        return NULL;
    }

    void *ptr = sa->memory + sa->top;  // Saving pointer to the current top position
    sa->top += bytes;  // Top updates

    return ptr;
}
// Function frees everything allocated after the given pointer.
// Comes in handy when trying to "unwind" back to a previous state:
static inline void stack_free_to(stack_allocator_t *sa, void *ptr) {
    sa->top = (uint8_t *)ptr - sa->memory;
}
// Function frees everyting at once (top is assigned back to 0):
static inline void stack_reset(stack_allocator_t *sa) {
    sa->top = 0;
}

#endif