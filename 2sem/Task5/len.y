%{
#include <iostream>
#include <stdio.h>
int yylex();
void yyerror(const char*);
%}
%%
P : S { std::cout << $1 << std::endl; }
S : '(' S ')' S { $$ = 1 + $4; }
  | { $$ = 0; }
%%
int yylex() {
	int c = getchar();
	if (c == '\n' || c == EOF) return 0;
	return c;
}
void yyerror(const char *s) { std::cerr << s << std::endl; }
int main() { yyparse(); return 0; }
