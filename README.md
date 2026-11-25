# C Data Structures & CLI Utilities

A modular C project featuring custom data structure implementations, dynamic memory management, and a multi-file CLI calculator using header files.

## Project Structure

- `01_dynamic_memory.c` — Dynamic array allocation, resizing with `realloc`, and leak-free cleanup
- `02_linked_list.c` — Singly linked list with insert, delete, search, and traversal operations
- `03_stack_and_queue.c` — Array-based Stack (LIFO) and Circular Queue (FIFO) implementations
- `04_string_utilities.c` — Custom string manipulation functions (palindrome check, word count, case toggle)
- `05_binary_search.c` — Iterative and recursive Binary Search with QuickSort preprocessing
- `calc.h` & `calc.c` & `06_modular_calculator.c` — Multi-file C program demonstrating header separation
- `Makefile` — Automated build rules for all executables

## Build & Run

```bash
make
./01_dynamic_memory
./06_modular_calculator
```
