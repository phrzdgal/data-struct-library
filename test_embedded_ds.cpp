#include <iostream>
#include <cassert>
#include "embedded_ds.h"
using namespace std;

// Test functions
void test_circular_buffer() {
    cout << "Testing Circular Buffer...\n";

    uint8_t storage[5];
    circular_buffer_t cb;
    cb_init(&cb, storage, 5);

    // Test empty
    assert(cb_is_empty(&cb) == true);
    assert(cb_is_full(&cb) == false);

    // Test write/read
    assert(cb_write(&cb, 42) == true);
    assert(cb_write(&cb, 100) == true);

    uint8_t data;
    // FIFO reading order --> first read data = 42, second read data = 100
    assert(cb_read(&cb, &data) == true && data == 42);
    assert(cb_read(&cb, &data) == true && data == 100);
    assert(cb_is_empty(&cb) == true);

    // Test full buffer
    for (int i = 0; i < 5; i++) {
        cb_write(&cb, i);
    }
    assert(cb_is_full(&cb) == true);
    assert(cb_write(&cb, 99) == false);

    cout << "Circular Buffer tests passed\n";
}
void test_stack_allocator() {
    cout << "Testing Stack Allocator...\n";

    uint8_t memory[100];
    stack_allocator_t stack;
    stack_init(&stack, memory, 100);

    // Test allocation
    void *ptr1 = stack_alloc(&stack, 20);
    assert(ptr1 != nullptr);
    void *ptr2 = stack_alloc(&stack, 30);
    assert(ptr2 != nullptr);

    // Test reset -- rewinding the allocator back to the beginning
    stack_reset(&stack);
    void *ptr3 = stack_alloc(&stack, 50);
    assert(ptr3 == memory);  // Should be back at start

    cout << "Stack Allocator tests passed\n";
}
void test_memory_pool() {
    cout << "Testing Memory Pool...\n";

    uint8_t memory[200];
    memory_pool_t pool;
    mp_init(&pool, memory, 200, 32);  // Blocks are 32-byte

    // Test allocation
    void *block1 = mp_alloc(&pool);
    assert(block1 != nullptr);
    void *block2 = mp_alloc(&pool);
    assert(block2 != nullptr);

    // Test free and reuse
    mp_free(&pool, block1);
    void *block3 = mp_alloc(&pool);
    assert(block3 == block1);

    cout << "Memory Pool tests passed...\n";
}
void test_hash_table() {
    cout << "Testing Hash Table...\n";

    uint8_t memory[5000];
    fixed_hash_table_t table;
    ht_init(&table, memory, 10, 16, 4);  // 10 buckets, 16-char keys, 4-byte values

    // Test put/get
    int temp = 25;
    assert(ht_put(&table, "temp", &temp) == true);

    int result;
    assert(ht_get(&table, "temp", &result) == true);
    assert(result == 25);

    // Test contains
    assert(ht_contains(&table, "temp") == true);
    assert(ht_contains(&table, "missing") == false);

    // Test remove
    assert(ht_remove(&table, "temp") == true);
    assert(ht_contains(&table, "temp") == false);

    cout << "Hash Table tests passed...\n";
}

int main() {
    cout << "Testing Embedded Data Structures...\n\n";

    test_circular_buffer();  // Calling test functions
    test_stack_allocator();
    test_memory_pool();
    test_hash_table();

    cout << "All tests passed! \n";
    return 0;
}