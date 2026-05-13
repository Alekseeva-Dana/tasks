%{
#include <iostream>
#include <ctype.h>
#include <stdio.h>
int yylex();
void yyerror(const char*);
%}
%token NUM
%left '+' '-'
%left '*' '/'
%%
S: E { std::cout << $1 << std::endl; }
E: E '+' E { $$ = $1 + $3; }
 | E '-' E { $$ = $1 - $3; }
| E '*' E { $$ = $1 * $3; }
| E '/' E { if ($3 != 0) $$ = $1 / $3; else yyerror("делить на ноль нельзя"); }
| '(' E ')' { $$ = $2; }
| NUM { $$ = $1; }
%%
int yylex() {
	int c;
	while ((c = getchar()) == ' ');
	if (isdigit(c)) {
		int v = 0;
 		while (isdigit(c)) { v = v * 10 + (c - '0'); c = getchar(); }
		ungetc(c, stdin);
		yylval = v;
		return NUM;
	}
	if (c == '\n' || c == EOF) return 0;
	return c;
}
void yyerror(const char *s) { std::cerr << s << std::endl; }
int main() { yyparse(); return 0; }
