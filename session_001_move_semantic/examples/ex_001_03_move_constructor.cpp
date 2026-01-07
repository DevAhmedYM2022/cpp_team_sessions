#include <iostream>
#include <vector>

class HeavyObject {
public:
	HeavyObject(size_t size) : data{ new unsigned char[size] }, size{ size } {
		std::cout << "HeavyObject created.\n";
	}

	HeavyObject(const HeavyObject& src) : data{ new unsigned char[src.size] }, size{ src.size } {
		std::cout << "HeavyObject created from copy.\n";
		std::copy(src.data, src.data, data);
	}

	HeavyObject(HeavyObject&& src) noexcept : data{ src.data }, size{ src.size } {
		std::cout << "HeavyObject created from move.\n";
		src.data = nullptr;
		src.size = 0;
	}

	// Add assignment operator for rule of five
	HeavyObject& operator=(const HeavyObject& src) {
		if (this != &src) {
			std::cout << "HeavyObject assigned from copy.\n";
			delete[] data;
			data = new unsigned char[src.size];
			size = src.size;
			memcpy(data, src.data, size);
		}
		return *this;
	}

	// Add assignment operator for rule of five
	HeavyObject& operator=(HeavyObject&& src) noexcept {
		if (this != &src) {
			std::cout << "HeavyObject assigned from move.\n";
			delete[] data;
			data = src.data;
			size = src.size;
			src.data = nullptr;
			src.size = 0;
		}
		return *this;
	}

	~HeavyObject() {
		if (data) {
			delete[] data;
		}
		data = nullptr;
		size = 0;
	}

public:
	unsigned char* data;
	size_t size;
};

HeavyObject create_object() {
	return HeavyObject(10); // This will cause move not copy
}

void process_object(HeavyObject obj) {

}

int main() {
	std::cout << __cplusplus << "\n";
	std::vector<HeavyObject> vec;
	auto obj = create_object();	// move
	vec.push_back(std::move(obj));	// neither copy, nor move just cast lvalue to rvalue

	HeavyObject h1(10);
	HeavyObject h2 = std::move(h1);		// Move
	h1 = std::move(h2);					// Move Assignment

	// process_object(h2);				// Invalid because h2 in invalid state
	process_object(std::move(h1));		// Move

	h1 = HeavyObject(20);				// Move Assignment (from rvalue)

	return 0;
}