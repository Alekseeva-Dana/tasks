#include <iostream>
using namespace std;

class B {
	int val;
public:
	wxplicit B(int x = 0, int y = 0) : val(x + y) {}

	B& operator+=(const B& other) {
		val += other.val;
		return *this;
	}

	int get() const { return val;}
};

int main() {
	B b1(6);
	B b2(2, 3);
	B b3(b1);

	b1 += b2 += b3;

	cout << b1.get() << ' ' << b2.get() << ' ' << b3.get() << endl;
	return 0;
}
