# Multithreading & Synchronization Library (C)

A custom synchronization and multithreading library implemented in C, designed to handle concurrency using atomic operations and low-level synchronization primitives without relying on standard OS-level blocking mechanisms.

## Features
- **Ticket Lock:** Implements strict FIFO ordering and bounded waiting as the foundational lock primitive.
- **Unbounded Semaphore:** Custom semaphore synchronization utilizing atomic primitives and busy-waiting/yielding strategies.
- **Writer-Preference Read-Write Lock:** Designed to prevent writer starvation under heavy concurrent loads.
- **Rigorous Testing Suite:** Validated through comprehensive unit, concurrent, and 16-thread stress test suites.

## Project Structure
- `PQ1/` - Basic synchronization primitives (Semaphore, ticket locks) and unit/stress tests.
- `PQ2/` - Condition variable implementation alongside core synchronization logic.
- `PQ3/` - Reader-Writer lock implementations with writer-preference mechanisms and parallel stress testing.

## Building and Running Tests
To compile and run the test suite, execute the automation script from the respective project directory:
```bash
./build_and_run_tests.sh
