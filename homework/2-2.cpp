//a
void f(int, const char* = nullptr);
void f(char, char);

//b
void f(const char* = nullptr);
void f(int, int = 0);

//c
void f(int, int);
void f(const char*, const char*);

//d
void f(const char*);
void f(int, const char* = nullptr);

//e
void f(const char*);
void f(long long, const char* = "");
