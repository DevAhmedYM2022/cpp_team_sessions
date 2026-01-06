# Move Semantics & Perfect Forwarding

## The Problem *Before C++11*
- When returning a heavy object, it calls the copy contructor of the object causing
	- Heap allocations.
	- Deep copy of data
	
	 Which causes extra execution time in performance-critical domains.
	> See [Example 1](./examples/ex_001_01_the_problem.cpp).
## Move