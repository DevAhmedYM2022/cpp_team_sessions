#include <string>
#include <iostream>

void foo(int& x) {}        // #1 - accepts lvalues only
void foo(int&& x) {}       // #2 - accepts rvalues only
void foo(const int& x) {}  // #3 - accepts both (const lvalue reference)

int main() {
	std::cout << __cplusplus << "\n";

	int a = 10;       // a is lvalue
	int& lref = a;    // lvalue reference
	int&& rref = 20;  // rvalue reference
	const int& clref = a;  // const lvalue reference (can bind to rvalues too)

	// int&& rref = a;		// --> invalid, because rvalue should point to temp value.

	int b = 5;
	foo(b);     // Calls #1
	foo(5);     // Calls #2
	foo(b + 3); // Calls #2
	const int c = 10;
	foo(c);     // Calls #3

	int x = 10;				// 'x' is an lvalue, '10' is an rvalue.
	std::cout << &x << "\n";
	// std::cout << &(x + 4) << "\n"; // --> cannot get address of the rvalue

	return 0;
}