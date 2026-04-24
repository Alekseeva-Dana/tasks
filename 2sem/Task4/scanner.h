#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <stdexcept>

enum TokenType {
    LEX_NULL,
    LEX_SELECT, LEX_INSERT, LEX_UPDATE, LEX_DELETE, LEX_CREATE, LEX_DROP,
    LEX_TABLE, LEX_FROM, LEX_WHERE, LEX_INTO, LEX_VALUES, LEX_SET, LEX_TEXT, LEX_LONG,
    LEX_AND, LEX_OR, LEX_NOT,
    LEX_COMMA, LEX_SEMICOLON, LEX_LEFT_PAREN, LEX_RIGHT_PAREN, LEX_STAR,
    LEX_EQ, LEX_LSS, LEX_GTR, LEX_LEQ, LEX_GEQ, LEX_NEQ,
    LEX_ID, LEX_NUM, LEX_STR,
    LEX_FINISH
};

struct Token {
    TokenType type;
    std::string value;
    int int_value;

    Token(TokenType t = LEX_NULL, std::string v = "", int n = 0) 
        : type(t), value(v), int_value(n) {}
};

class ScannerException : public std::runtime_error {
public:
    ScannerException(const std::string& msg) : std::runtime_error("Scanner Error: " + msg) {}
};

class Scanner {
    std::string source;
    size_t pos;

    static const std::string keywords[];
    static const TokenType key_tokens[];

    void skip_spaces();
    Token look_up(const std::string& s);

public:
    Scanner(const std::string& input);
    Token get_token();
};

#endif
