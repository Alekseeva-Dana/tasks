#include "scanner.h"
#include <cctype>

const std::string Scanner::keywords[] = {
    "SELECT", "INSERT", "UPDATE", "DELETE", "CREATE", "DROP",
    "TABLE", "FROM", "WHERE", "INTO", "VALUES", "SET", "TEXT", "LONG",
    "AND", "OR", "NOT", "" 
};

const TokenType Scanner::key_tokens[] = {
    LEX_SELECT, LEX_INSERT, LEX_UPDATE, LEX_DELETE, LEX_CREATE, LEX_DROP,
    LEX_TABLE, LEX_FROM, LEX_WHERE, LEX_INTO, LEX_VALUES, LEX_SET, LEX_TEXT, LEX_LONG,
    LEX_AND, LEX_OR, LEX_NOT
};

Scanner::Scanner(const std::string& input) : source(input), pos(0) {}

void Scanner::skip_spaces() {
    while (pos < source.length() && isspace(source[pos])) pos++;
}

Token Scanner::look_up(const std::string& s) {
    std::string upper_s = s;
    for (auto &ch : upper_s) ch = toupper(ch);
    for (int i = 0; !keywords[i].empty(); ++i) {
        if (upper_s == keywords[i]) return Token(key_tokens[i], s);
    }
    return Token(LEX_ID, s);
}

Token Scanner::get_token() {
    skip_spaces();
    if (pos >= source.length()) return Token(LEX_FINISH, "EOF");

    char c = source[pos];

    if (isalpha(c)) {
        std::string res;
        while (pos < source.length() && (isalnum(source[pos]) || source[pos] == '_')) {
            res += source[pos++];
        }
        return look_up(res);
    }

    if (isdigit(c)) {
        int num = 0;
        while (pos < source.length() && isdigit(source[pos])) {
            num = num * 10 + (source[pos++] - '0');
        }
        return Token(LEX_NUM, std::to_string(num), num);
    }

    if (c == '\'') {
        pos++;
        std::string res;
        while (pos < source.length() && source[pos] != '\'') {
            res += source[pos++];
        }
        if (pos == source.length()) throw ScannerException("Unclosed string literal");
        pos++;
        return Token(LEX_STR, res);
    }

    pos++;
    switch (c) {
        case ',': return Token(LEX_COMMA, ",");
        case ';': return Token(LEX_SEMICOLON, ";");
        case '*': return Token(LEX_STAR, "*");
        case '(': return Token(LEX_LEFT_PAREN, "(");
        case ')': return Token(LEX_RIGHT_PAREN, ")");
        case '=': return Token(LEX_EQ, "=");
        case '<':
            if (pos < source.length() && source[pos] == '=') { pos++; return Token(LEX_LEQ, "<="); }
            if (pos < source.length() && source[pos] == '>') { pos++; return Token(LEX_NEQ, "<>"); }
            return Token(LEX_LSS, "<");
        case '>':
            if (pos < source.length() && source[pos] == '=') { pos++; return Token(LEX_GEQ, ">="); }
            return Token(LEX_GTR, ">");
        default:
            throw ScannerException(std::string("Unknown character: ") + c);
    }
}
