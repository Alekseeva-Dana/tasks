class cat{
	double weight;
	char name[100];

	friend int f(const cat c1, const cat c2, const cat& c3, const cat& c4);
public:
	cat(const char* n, double w = 12.0) : weight(w) {
		strcpy(name, n);
	}
	cat(double w) : weight(w) {
		strcpy(name, "cat");
	}
};

//временный объект t1, временный объект t2, временный объект t3, временный объект t4,
//копия t1, копия t2, деструкторы для c1 и c2, деструкторы для t1, t2, t3, t4
