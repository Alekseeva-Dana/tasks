#include <iostream>
using namespace std;

class A {
	int value;
public:
	A(int v = 0) : value(v) {}

	A& operator*=(int x) {
		value *= x;
		return *this;
	}

	A& operator*=(const A& other) {
		value *= other.value;
		return *this;
	}
	
	int get() const {
		return value;
	}
};

int main() {
	A a1(5), a2 = 3;
	a1 *= 10;
	a2 *= a1 *= 2;
	cout << a1.get() << " " << a2.get() << endl;
	return 0;
}
