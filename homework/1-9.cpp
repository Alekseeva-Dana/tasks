#include <iostream>
using namespace std;

class C {
	int val;
public:
	explicit C(int x = 0) : val(x) {}
	C operator+(const C& other) const {
		return C(val + other.val);
	}

	int get() const {
		return val * 2;
	}
};

int main() {
	C c1(7), c3(c1 + c1);

	//c2 = 5;
	cout << c1.get() << ' ' << c3.get() << endl;
	return 0;
}
	
