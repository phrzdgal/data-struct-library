# Memory-Constrained Data Structure Library

Implementation of embedded-optimized versions of common data structures (circular buffers, memory pools, fixed-size hash tables, stack allocators) with explicit memory management and no dynamic allocation. 

## Key Principles

### Memory Management:
- No malloc/free: Many embedded systems often don't use a heap, and dynamic allocation causes unpredictable timing, memory. fragmentation, and potential failures during critical operations.
- Predictable memory usage: Regulatory approval, safety analysis, and resource planning all require knowing exact memory usage at compile time.
- Memory alignment: To avoid faults and the slowness of unaligned access. 

### Performance & Constraints:
- Interrupt safety with volatile: Often the main code may be reading a buffer while an interrupt handler is writing to it. Without volatile, the compiler may optimize away reads, causing data corruption or infinite loops.
- Cache-friendly layouts: Keeping related data together so the CPU loads useful data in each cache line. 
- Deterministic Performance: Algorithm should take the same amount of time each time it is run - embedded systems must often respond within microseconds every single time. 

### Design Patterns:
- Caller-provided buffers: Caller gets better conrols and keeps library from making memory decisions it shouldn't. 
- Error codes vs exceptions: Exception handling adds unpredictable timing and code size overhead. Many embedded compilers don't even support exceptions. Opting for error codes instead. 

