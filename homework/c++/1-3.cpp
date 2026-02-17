//пункт а
class X {
	int i;
	double t;
	X(int k) {
		i = k;
		t = 0;
		cout << 1;
	}
public:
	X(int k, double r = 0) {
		i = k;
		t = r;
		cout << 2;
	}
	X & operator = (X & a) {
		i = a.i;
		t = a.t;
		cout << 3;
		return * this;
	}
	X(const X & a) {
		i = a.i;
		t = a.t;
		cout << 4;
	}
};

int main() { //все конструкции в main ошибочны
	X.a;
	X b(1);
	X c (2, 3.5);
	X d = c;
	X e (6.5, 3);
	c = d = e;
	return 0;
}

//пункт б
class X {
	int i;
	double t;
	X() {
		i = 0;
		t = 1.0;
		cout << 1;
	}
public:
	X(int k = 0, double r = 1.5) {
		i = k;
		t = r;
		cout << 2;
	}
	X(const X & a) {
		i = a.i;
		t = a.t;
		cout << 3;
	}
};

int main() { //будет выведено: 2232
	X a; //убрать это
	X b(1);
	X c (1.5, 2);
	X d = b;
	X e = 3;
	b = c = e;
	return 0;
}
