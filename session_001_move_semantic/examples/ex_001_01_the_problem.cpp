#include <iostream>

class HeavyObject {
public:
	HeavyObject(size_t size) : data{ new unsigned char[size] }, size{ size } {
		std::cout << "HeavyObject created.\n";
	}

	HeavyObject(const HeavyObject& src) : data{ new unsigned char[src.size] }, size{ src.size } {
		std::cout << "HeavyObject created from copy.\n";
		std::copy(src.data, src.data, data);
	}

	~HeavyObject() {
		delete[] data;
		data = nullptr;
		size = 0;
	}

public:
	unsigned char* data;
	size_t size;
};

HeavyObject update(HeavyObject obj) {
	HeavyObject updated_obj = obj;
	updated_obj.data[0] = 0;
	return updated_obj;
}

int main() {
	HeavyObject obj(1920 * 1080 * 3);
	HeavyObject updated = update(obj);

	return 0;
}