#include <vector>
#include <iostream>
#include <print>

class LargeObject {
	std::vector<int> data;  // 1MB of data
public:
	LargeObject(size_t size) : data(size, 0) {}

	// Copy constructor - expensive!
	LargeObject(const LargeObject& other) : data(other.data) {
		std::cout << "Copying 1MB data!\n";
	}

	// Move constructor - cheap!
	LargeObject(LargeObject&& other) noexcept : data(std::move(other.data)) {
		std::cout << "Moving 1MB data!\n";
	}
};

std::vector<LargeObject> storage;

// Test function
template<typename T>
void process_without_forwarding(const T& obj) {
	storage.push_back(obj);  // ALWAYS COPIES - 1MB copy!
}

template<typename T>
void process_with_forwarding(T&& obj) {
	storage.push_back(std::forward<T>(obj));  // COPIES or MOVES based on input
}

template<typename T>
void process_with_forwarding2(T&& obj) {
	storage.push_back(std::move(obj));  // Always move
}

int main() {
	// Usage comparison:
	LargeObject obj1(1000000);  // 1MB object
	LargeObject obj2(1000000);
	LargeObject obj3(1000000);

	process_without_forwarding(obj1);        // 1MB COPY (unavoidable)
	process_without_forwarding(std::move(obj2)); // STILL 1MB COPY! (wasted opportunity)

	process_with_forwarding(obj1);           // 1MB Copy (necessary)
	process_with_forwarding(std::move(obj2)); // 1MB MOVE (efficient!)
	process_with_forwarding(LargeObject(1000000)); // 1MB MOVE (efficient!)

	process_with_forwarding2(obj1);           // 1MB MOVE but want to keep obj1
	process_with_forwarding2(std::move(obj3)); // 1MB MOVE (efficient!)
	process_with_forwarding2(LargeObject(1000000)); // 1MB MOVE (efficient!)
	return 0;
}