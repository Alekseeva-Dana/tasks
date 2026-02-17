class A {
	int a, b;
public: 
	A (A & x) {
		a = x.a;
		b = x.b;
		cout << 1;
	}
	A (int a, int b = 0) { //исправили этот конструктор
		this -> a = a;
		this -> b = b;
		cout << 2;
	}
};

void f() {
	A x (1);
	A y;
	A z = A (2.5, 4);
	A s = 6;
	A w = z;
	x = z = w;
}
//будет выведено: 22221
