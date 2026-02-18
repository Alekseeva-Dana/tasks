//1 ошибка
my_str(const char* s) : len(strken(s)) {
	str = new char[len + 1];
	strcpy(str, s);

//2 ошибка
::strcpy(str, t.str);

//3 ошибка
#include <cstring>

//4 ошибка
в main при вызове c.strcpy(a); нельзя вызывать константный объект

//5 ошибка
strcpy(a.str, b.str);
a.str и b.str - приватные поля, недоступные из main
