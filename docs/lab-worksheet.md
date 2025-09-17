# Memory-Constrained Data Structure Library for Embedded Systems

## Project Overview

This project implements embedded-optimized versions of common data structures with explicit memory management and no dynamic allocation. The library demonstrates understanding of embedded systems constraints and memory management principles critical for resource-constrained environments.

## Learning Objectives

- **Memory Management**: Understanding stack vs heap, memory-mapped I/O concepts, and why dynamic allocation is forbidden in embedded systems
- **Embedded Constraints**: Limited RAM, predictable memory usage, and deterministic performance requirements
- **Data Structure Optimization**: Adapting traditional CS data structures to embedded realities
- **Systems Programming**: C/C++ pointer arithmetic, memory layouts, and low-level optimization

## Technical Requirements

### Core Constraints
- **No Dynamic Allocation**: No `malloc()`, `free()`, `new`, or `delete`
- **Predictable Memory Usage**: All memory requirements known at compile time
- **Caller-Provided Memory**: Library never allocates its own memory
- **Deterministic Performance**: All operations must have predictable timing
- **Thread/Interrupt Safety**: Use `volatile` where appropriate for ISR compatibility

### Implementation Standards
- **Language**: C++ with C-style embedded practices
- **Style**: Header-only library using `static inline` functions
- **Memory Alignment**: Consider CPU architecture alignment requirements
- **Error Handling**: Return codes instead of exceptions
- **Documentation**: Clear memory requirements and usage examples

## Deliverables

### 1. Circular Buffer (Ring Buffer)
**Purpose**: Handle streaming data with fixed memory footprint

**Key Features**:
- FIFO (First In, First Out) behavior
- Wrap-around when reaching end of buffer
- Interrupt-safe operations with `volatile` indices
- Overflow detection and handling

**Core Functions**:
```c
void cb_init(circular_buffer_t *cb, uint8_t *buffer, size_t size);
bool cb_write(circular_buffer_t *cb, uint8_t data);
bool cb_read(circular_buffer_t *cb, uint8_t *data);
bool cb_is_empty(circular_buffer_t *cb);
bool cb_is_full(circular_buffer_t *cb);
size_t cb_available_space(circular_buffer_t *cb);
size_t cb_data_count(circular_buffer_t *cb);
```

**Real-world Applications**: UART buffers, sensor data queues, audio sample buffers

### 2. Stack Allocator
**Purpose**: Fast temporary memory allocation with LIFO deallocation pattern

**Key Features**:
- Linear allocation with moving "top" pointer
- Bulk deallocation (reset entire stack)
- Nested allocation patterns with free-to operations
- Zero fragmentation

**Core Functions**:
```c
void stack_init(stack_allocator_t *sa, uint8_t *memory, size_t size);
void* stack_alloc(stack_allocator_t *sa, size_t bytes);
void stack_free_to(stack_allocator_t *sa, void *ptr);
void stack_reset(stack_allocator_t *sa);
```

**Real-world Applications**: Per-frame allocations, function call scratch space, temporary buffers

### 3. Memory Pool
**Purpose**: Fixed-size block allocator for object lifecycle management

**Key Features**:
- All blocks identical size for zero fragmentation
- Free list management using blocks themselves
- Individual allocation and deallocation
- Memory reuse and recycling

**Core Functions**:
```c
void mp_init(memory_pool_t *mp, uint8_t *memory, size_t total_size, size_t block_size);
void* mp_alloc(memory_pool_t *mp);
void mp_free(memory_pool_t *mp, void *block);
```

**Real-world Applications**: Network packet allocation, sensor object management, game entity pools

### 4. Fixed Hash Table
**Purpose**: Key-value storage with predictable memory usage

**Key Features**:
- Fixed-size buckets with no dynamic collision chains
- Linear probing for collision resolution
- String keys with configurable maximum length
- Fixed-size values for predictable storage

**Core Functions**:
```c
void ht_init(fixed_hash_table_t *ht, uint8_t *memory, size_t table_size, 
             size_t max_key_length, size_t value_size);
bool ht_put(fixed_hash_table_t *ht, const char *key, const void *value);
bool ht_get(fixed_hash_table_t *ht, const char *key, void *value);
bool ht_remove(fixed_hash_table_t *ht, const char *key);
bool ht_contains(fixed_hash_table_t *ht, const char *key);
```

**Real-world Applications**: Configuration storage, sensor lookup tables, command dispatch

### 5. Master Header
**File**: `embedded_ds.h`
```c
#ifndef EMBEDDED_DS_H
#define EMBEDDED_DS_H

#include "circular_buffer.h"
#include "memory_pool.h" 
#include "fixed_hash_table.h"
#include "stack_allocator.h"

#endif // EMBEDDED_DS_H
```

### 6. Comprehensive Test Suite
**File**: `test_embedded_ds.cpp`

**Test Coverage**:
- Basic functionality for all data structures
- Edge cases (empty, full, overflow conditions)
- Memory reuse verification
- FIFO/LIFO behavior validation
- Error handling verification

**Test Structure**:
```cpp
void test_circular_buffer();
void test_stack_allocator();
void test_memory_pool();
void test_hash_table();
int main(); // Runs all tests
```

## Design Decisions and Rationale

### Memory Management Philosophy
- **Caller Responsibility**: Applications control memory allocation and sizing
- **Compile-Time Sizing**: All memory requirements deterministic
- **Zero Hidden Costs**: No malloc overhead or unpredictable allocations
- **Resource Ownership**: Clear ownership model prevents memory leaks

### Performance Characteristics
| Data Structure | Allocation | Access | Search | Space Overhead |
|----------------|------------|--------|--------|----------------|
| Circular Buffer | O(1) | O(1) | N/A | Minimal |
| Stack Allocator | O(1) | N/A | N/A | Pointer only |
| Memory Pool | O(1) | N/A | N/A | Free list |
| Hash Table | O(1) avg | O(1) avg | O(1) avg | Bucket overhead |

### Thread Safety Considerations
- Use `volatile` for variables accessed in ISRs
- Atomic operations where required by architecture
- Clear documentation of thread safety guarantees
- Lock-free designs where possible

## Interview Relevance

### Technical Concepts Demonstrated
- **Memory Management**: Stack vs heap, alignment, fragmentation
- **Systems Programming**: Pointer arithmetic, memory layouts, optimization
- **Embedded Constraints**: Real-time requirements, resource limitations
- **Data Structure Design**: Adapting algorithms to constraints

### Common Interview Questions Addressed
- "Implement a memory allocator without malloc"
- "Design a data structure for embedded systems"
- "How would you handle memory constraints?"
- "Explain trade-offs between different allocation strategies"

### Big O Analysis
All operations designed for predictable, constant-time performance suitable for real-time embedded applications.

## File Structure
```
embedded_data_structures/
├── README.md                    # Project documentation
├── embedded_ds.h                # Master header
├── circular_buffer.h            # Ring buffer implementation
├── stack_allocator.h            # Linear allocator
├── memory_pool.h                # Block allocator
├── fixed_hash_table.h           # Hash table with linear probing
└── test_embedded_ds.cpp         # Comprehensive test suite
```

## Compilation and Testing
```bash
# Compile tests
g++ test_embedded_ds.cpp -o test

# Run test suite
./test
```

**Expected Output**:
```
Testing Embedded Data Structures...

Testing Circular Buffer...
Circular Buffer tests passed
Testing Stack Allocator...
Stack Allocator tests passed
Testing Memory Pool...
Memory Pool tests passed
Testing Hash Table...
Hash Table tests passed

All tests passed! ✅
```

## Success Criteria
- [X] All four data structures implemented correctly
- [X] Comprehensive test suite passes without errors
- [X] No dynamic memory allocation anywhere in codebase
- [X] Clear documentation of memory requirements
- [X] Professional code organization and style
- [X] Demonstrates embedded systems best practices

This project showcases the ability to bridge computer science fundamentals with embedded systems constraints, making it highly relevant for embedded systems engineering roles.