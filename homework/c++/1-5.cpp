#include <iostream>
using namespace std;

class B {
	int value;
public:
	B() : value(10) {}
	B(const B& other) : value(other.value + 10) {}
	int get() const { return value; }
};
