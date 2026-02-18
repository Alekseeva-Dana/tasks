class fraction {
	int num, den;

	void reduce() { //сокращение дроби
		int a = num, b = den;
		while (b) {
			int t = a % b;
			a = b;
			b = t;
		}
		int gcd = a;
		num /= gcd;
		den /= gcd;

		if (den < 0) {
			num = -num;
			den = -den;
		}
	}
public:
	fraction(int n, int d) : num(n), den(d) {
		reduce();
	}

	fraction(int n) : num(n), den(1) {}

	fraction() : num(0), den(1) {}
};

//нет, для создания копий объектов не нужно явно определять конструктор копирования
