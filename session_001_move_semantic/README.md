# Move Semantics & Perfect Forwarding
Modern C++ (C++11 and beyond) introduced a paradigm shift in how we handle objects. Instead of always copying data—which is expensive—we can now move resources from one object to another.

## Old way (copy only)
> See [Example 1](./examples/ex_001_01_the_problem.cpp).

Before C++11, if you wanted to pass a temporary object (like a returned vector) into another function or container, the compiler had to deep copy the data.
- The Problem: High performance overhead. For a std::vector with a million elements, a copy requires allocating new memory and copying every single element, even if the source object is about to be destroyed.
- The Syntax: Relied entirely on the Copy Constructor T(const T&) and Copy Assignment operator=(const T&).

### Real-case
```
std::map<string, string> loadConfig() {
    std::map<string, string> config;
    // ... fill with 10,000 settings ...
    return config; 
}

// In C++98, 'currentConfig' would allocate new memory and 
// copy every string from the temporary returned map.
std::map<string, string> currentConfig = loadConfig();
```

## lvalue vs rvalue
> See [Example 2](./examples/ex_001_02_lvalue_rvalue.cpp).

To understand "moving," you must understand value categories.
```
int x = 10;           // 'x' is an lvalue, '10' is an rvalue.
int y = x + 5;        // 'x + 5' is a temporary rvalue.
```

|Category|Definition|Simple Test|
|--------|----------|-----------|
|lvalue|An object that occupies a clear memory location (has an address).|Can you take its address with &? Yes.|
|rvalue|A temporary value that does not persist beyond the expression.|Can you take its address? No.|

### Real-case
- Preventing a developer from accidentally binding a reference to a temporary object that will be destroyed.
```
void logMessage(std::string& msg);  // Only accepts lvalues (variables)
void logMessage(std::string&& msg); // Only accepts rvalues (temporaries)

std::string report = "Error 404";
logMessage(report);           // Calls lvalue version
logMessage("Direct Error");   // Calls rvalue version (efficient, no copy needed)
```

## Move semantics
> See [Example 3](./examples/ex_001_03_move_constructor.cpp)

Move semantics allows an object to "steal" the resources (pointers, file handles, sockets) of an rvalue instead of copying them.
	
1. The Rvalue Reference (&&):
	- We use && to detect rvalues. This allows us to create overloads specifically for temporary objects.
1. Move Constructor & Assignment:
	- Instead of allocating new memory, we simply point to the existing data and set the source's pointer to nullptr. 
	```
	HeavyObject(HeavyObject&& src) noexcept : data{ src.data }, size{ src.size } {
		std::cout << "HeavyObject created from move.\n";
		src.data = nullptr;
		src.size = 0;
	}
	```
1. ```std::move```:
	- ```std::move``` doesn't actually move anything; it is a type cast. It turns an lvalue into an rvalue so that the move constructor can be triggered.


### Real-case
- You have a VideoFrame class holding 10MB of pixel data. You want to pass it from the Capture module to the Encoder module without copying the 10MB.
```
class VideoFrame {
    uint8_t* pixels;
public:
    // Move constructor: Just swap the pointers
    VideoFrame(VideoFrame&& other) noexcept {
        this->pixels = other.pixels;
        other.pixels = nullptr; // Encoder now owns the data; Capture owns nothing
    }
};
```
## Perfect forwarding
> See [Example 4](./examples/ex_001_04_perfect_forward.cpp)

When writing templates, we often want to pass arguments to another function exactly as they were received (keeping their "lvalueness" or "rvalueness").

- The Problem: Reference Decay:
	- If you take an argument ```T arg```, it's always a copy. If you take ```T& arg```, it's always an lvalue. You lose the "identity" of the original argument.
- The Solution: Universal References & std::forward
	- Using ```T&&``` in a template context creates a Forwarding Reference.
	```
	template<typename T>
	void wrapper(T&& arg) {
		// std::forward restores the original value category
		actual_function(std::forward<T>(arg));
	}
	```

### Real-case
- You want to write a createInstance<T>(args...) function that works for any class, regardless of whether the constructor arguments are passed by reference or by value.
```
template<typename T, typename... Args>
std::unique_ptr<T> create(Args&&... args) {
    // std::forward ensures rvalues stay rvalues, lvalues stay lvalues
    return std::make_unique<T>(std::forward<Args>(args)...);
}
```
## How STL work with move semantics
> See [Example 5](./examples/ex_001_05_stl_move_aware.cpp)

The Standard Template Library was redesigned to be "move-aware."
- ```std::vector::push_back```: Has an rvalue overload. If you pass a temporary, it moves it into the heap memory of the vector.
- Reallocation: When a vector grows, it will move the existing elements to the new memory block instead of copying them (provided your move constructor is marked noexcept).
- ```std::unique_ptr```: This type is move-only. It cannot be copied, ensuring single ownership of a resource.

## Rule of three, five, and zero.
### Rule of Three (Pre-C++11)
1. If you need to explicitly declare any of:
	- Destructor
	- Copy constructor
	- Copy assignment operator
	
	Then you probably need to declare all three.

### Rule of Five (C++11 and later)
1. With move semantics, the rule extends to five special member functions:
	- Destructor
	- Copy constructor
	- Copy assignment operator
	- Move constructor
	- Move assignment operator

### Rule of Zero
1. The ideal modern C++ approach: Classes should not define any special member functions.
```
// Rule of Zero - let the compiler handle everything
class RuleOfZero {
    std::unique_ptr<int> data;  // Manages resource
    std::string name;           // Copyable/movable by default
    std::vector<double> values; // Copyable/movable by default
    
public:
    RuleOfZero(int value, std::string n, std::vector<double> v)
        : data(std::make_unique<int>(value))
        , name(std::move(n))
        , values(std::move(v)) {}
    
    // No need for destructor, copy/move operations
    // Compiler generates correct defaults
};

// All special members are correctly defined:
// Destructor: calls member destructors
// Copy: deep copy via unique_ptr's deleted copy operations
// Move: transfers ownership automatically
```