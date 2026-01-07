#include <iostream>
#include <vector>

class HeavyObject {
public:
    HeavyObject(size_t size) : data(new unsigned char[size]), size(size) {
        std::cout << "HeavyObject created.\n";
    }

    HeavyObject(const HeavyObject& src) : data(new unsigned char[src.size]), size(src.size) {
        std::cout << "HeavyObject created from copy.\n";
        memcpy(data, src.data, size);
    }

    ~HeavyObject() {
        if (data) {
            delete[] data;
        }
        data = 0;  // nullptr is C++11
        size = 0;
    }

    // Add assignment operator for rule of three
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

public:
    unsigned char* data;
    size_t size;
};

HeavyObject create_object() {
    return HeavyObject(10);  // This will cause copy in C++98, not move
}

void process_object(HeavyObject obj) {

}

int main() {
    std::cout << __cplusplus << "\n";
    std::vector<HeavyObject> vec;
    vec.push_back(create_object());  // Will copy, not move
    
    HeavyObject h1(10);
    HeavyObject h2 = h1;    // Copy
    h1 = h2;                // Copy Assignment
    process_object(h2);     // Copy
    return 0;
}