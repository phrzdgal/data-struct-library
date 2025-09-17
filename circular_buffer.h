// Queue, but end loops back to the beginning like in a circle. 
// CBs come useful in real-time streaming data, producer-consumer scenarios, or when 
// there is fixed memory + continous data. 
// To search for data, O(n) runtime. Many functions outlined below are O(1) runtime. 
// Data container --> Focus on how data is stored and accessed
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stdbool.h> 
#include <stddef.h>
using namespace std;

// Struct defined here:
typedef struct {
    uint8_t *buffer;   // Pointer to buffer/data storage
    size_t size;       // Size of entire buffer
    size_t head;       // Write Index
    size_t tail;       // Read Index
    size_t count;      // Number of elements currently in buffer

} circular_buffer_t;

// Function Declarations:
// Core functionality - to initialize the buffer
static inline void cb_init(circular_buffer_t *cb, uint8_t *buffer, size_t size);
// To check whether buffer is empty
static inline bool cb_is_empty(circular_buffer_t *cb);
// To check whether buffer is full
static inline bool cb_is_full(circular_buffer_t *cb);
// Core functionality - writing to the buffer
static inline bool cb_write(circular_buffer_t *cb, uint8_t data); // bool to return success or failure
// Core functionality - reading to the buffer
static inline bool cb_read(circular_buffer_t *cb, uint8_t *data); // Returns success, data via pointer

// Optional yet useful functions
static inline size_t cb_available_space(circular_buffer_t *cb);  // How much room is left?
static inline size_t cb_data_count(circular_buffer_t *cb);  // How much data waiting?


// Function Implementations:
// Function sets up an initial empty buffer, connects struct to the caller's buffer,
// and stores the buffer size. 
static inline void cb_init(circular_buffer_t *cb, uint8_t *buffer, size_t size) {
    cb->buffer = buffer;
    cb->size = size;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

static inline bool cb_is_empty(circular_buffer_t *cb) {
    return cb->count == 0;
}

static inline bool cb_is_full(circular_buffer_t *cb) {
    return cb->count == cb->size;
}

// Following FIFO Order - add from the head, read from the tail
static inline bool cb_write(circular_buffer_t *cb, uint8_t data) {
    // Tail points to the oldest data, head points to new data
    if (cb_is_full(cb)) {
        return false;
    } else {
        cb->buffer[cb->head] = data;  // Store data at current head position
        cb->head = (cb->head + 1) % cb->size;  // Move head to next position + wrap-around

        cb->count += 1;
        return true;
    }
}

// Read from the tail
static inline bool cb_read(circular_buffer_t *cb, uint8_t *data) {
    if (cb_is_empty(cb)) {
        return false;
    } else {
        *data = cb->buffer[cb->tail];
        cb->tail = (cb->tail + 1) % cb->size;
        cb->count--;
        return true;
    }
}


static inline size_t cb_available_space(circular_buffer_t *cb) {
    return cb->size - cb->count;
}

static inline size_t cb_data_count(circular_buffer_t *cb) {
    return cb->count;
}

#endif 