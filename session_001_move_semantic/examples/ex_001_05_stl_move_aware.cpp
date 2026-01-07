#include <vector>
#include <iostream>
#include <string>

class HeavyObject {
public:
	HeavyObject() : data{ nullptr }, size{ 0 } {}

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

std::vector<HeavyObject> createVector() {
    std::vector<HeavyObject> v;
    v.reserve(100);
    for (int i = 0; i < 3; ++i) {
        v.push_back(HeavyObject(i));
    }
    return v;  // NRVO or move
}

int main() {
    // Container operations that benefit from move semantics:

    std::vector<HeavyObject> v1 = createVector();  // Move construction
    std::vector<HeavyObject> v2;
    v2 = std::move(v1);  // Move assignment

	HeavyObject h(10);
    v2.emplace_back(10);  // Construct in-place (perfect forwarding)
    v2.push_back(std::move(h));  // Move push

    // std::vector::resize with move
    v2.resize(1000);  // Might move elements to new storage

	return 0;
}