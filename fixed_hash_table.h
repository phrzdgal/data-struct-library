// Fixed Hash Table = Dictionary/Map with Pre-allocated Memory
// Is a data structure that stores key-value pairs (like a dictionary) using a hash 
// function to quickly find where data is stored, but with fixed memory (no malloc)
#ifndef FIXED_HASH_TABLE_H
#define FIXED_HASH_TABLE_H

#include <stdint.h>
#include <stdbool.h> 
#include <stddef.h>
#include <string.h>
using namespace std;

typedef struct {
    uint8_t *memory;    // Pointer to storage array
    size_t table_size;  // User provided table size information (# of buckets/slots)
    size_t max_key_length;  // Max characters in a key
    size_t value_size;  // Size of each value (bytes)
    size_t bucket_size;  // Total size of a single bucket (key + value + flags)
} fixed_hash_table_t;

// Function Declarations:
static inline void ht_init(fixed_hash_table_t *ht, uint8_t *memory, size_t table_size, size_t max_key_length, size_t value_size);
static inline bool ht_put(fixed_hash_table_t *ht, const char *key, const void *value);
static inline bool ht_get(fixed_hash_table_t *ht, const char *key, void *value);
static inline bool ht_remove(fixed_hash_table_t *ht, const char *key);

static inline bool ht_contains(fixed_hash_table_t *ht, const char *key);

// Function Implementations:
static inline void ht_init(fixed_hash_table_t *ht, uint8_t *memory, size_t table_size, size_t max_key_length, size_t value_size) {
    ht->memory = memory;
    ht->table_size = table_size;
    ht->max_key_length = max_key_length;
    ht->value_size = value_size;
    ht->bucket_size = max_key_length + value_size + 1;

    memset(memory, 0, table_size * ht->bucket_size);
}

// Hash function below in order to store key-value pairs:
static inline size_t hash_function(const char *key) {
    size_t hash = 0;
    while (*key) {
        hash = hash * 31 + *key; // Multiply by a prime (helps distribute keys evenly), then add character's ASCII value
        key++;  // Move pointer to next character in string
    }
    return hash;
}

// Function below must hash the key to find which bucket to use, handle collisions, and 
// store the key-value pair in the bucket:
static inline bool ht_put(fixed_hash_table_t *ht, const char *key, const void *value) {
    size_t index = hash_function(key) % ht->table_size;  // Get starting bucket index

    // Implementing linear probing collision strategy
    for (size_t i = 0; i < ht->table_size; i++) {
        size_t current_index = (index + i) % ht->table_size;  // Wrap around...until space is found

        // Calculate where this bucket is in memory (Get bucket address)
        u_int8_t *bucket = ht->memory + (current_index * ht->bucket_size);
        // Find the "occupied" flag
        u_int8_t *occupied_flag = bucket + ht->max_key_length + ht->value_size;

        // Check if bucket is empty or contains the same key (update case)
        if (*occupied_flag == 0 || strcmp((char*)bucket, key) == 0) {
            // Store the key (copy string into bucket)
            strncpy((char *)bucket, key, ht->max_key_length - 1);
            ((char *)bucket)[ht->max_key_length - 1] = '\0';  // Ensure null termination

            // Store value (copy value data)
            uint8_t *value_location = bucket + ht->max_key_length;
            memcpy(value_location, value, ht->value_size);

            // Mark as occupied
            *occupied_flag = 1;

            return true;  // Success
        }
    }

    return false;  // Table is full
}
// Function retrieves a value by its key
// Returns true if key found, false if not found
static inline bool ht_get(fixed_hash_table_t *ht, const char *key, void *value) {
    size_t index = hash_function(key) % ht->table_size;

    // Linear probing to find key
    for (size_t i = 0; i < ht->table_size; i++) {
        size_t current_index = (index + i) % ht->table_size;
        u_int8_t *bucket = ht->memory + (current_index * ht->bucket_size);
        u_int8_t *occupied_flag = bucket + ht->max_key_length + ht->value_size;

        // If bucket is empty, key DNE
        if (*occupied_flag == 0) {
            return false;
        }

        // If key matches, copy value and return 
        if (strcmp((char*)bucket, key) == 0) {
            uint8_t *value_location = bucket + ht->max_key_length;
            memcpy(value, value_location, ht->value_size);
            return true;
        }
    }

    return false;  // Key not found
}
// Function deletes a key-value pair
// Returns true if key was found and removed, false if key did not exist
static inline bool ht_remove(fixed_hash_table_t *ht, const char *key) {
    size_t index = hash_function(key) % ht->table_size;

    for (size_t i = 0; i < ht->table_size; i++) {
        size_t current_index = (index + i) % ht->table_size;
        u_int8_t *bucket = ht->memory + (current_index * ht->bucket_size);
        u_int8_t *occupied_flag = bucket + ht->max_key_length + ht->value_size;

        if (*occupied_flag == 0) {
            return false;  // Key does not exist
        }

        if (strcmp((char*)bucket, key) == 0) {
            // *occupied_flag = 0; old data just sits there but can now be overwritten
            *occupied_flag = 0; // Mark as empty
            return true;
        }
    }

    return false;
}
// Function checks if a key exists but doesn't care about the value
static inline bool ht_contains(fixed_hash_table_t *ht, const char *key) {
    uint8_t dummy[16];  // Dummy storage
    return ht_get(ht, key, dummy);
}

#endif