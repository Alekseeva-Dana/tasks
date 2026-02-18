class complex {
	double re, im;
};

//пункт а
complex(double r, double i) : re(r), im(i) {}

//пункт б
complex(double r) : re(r), im(0) {}

//пункт в
complex() : re(0), im(0) {}

//да, такой конструктор написать можно
