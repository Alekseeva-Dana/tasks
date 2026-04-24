#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

class ParserException : public std::runtime_error {
public:
    ParserException(const std::string& msg) : std::runtime_error("Syntax Error: " + msg) {}
};

class Parser {
    Scanner &scanner;
    Token curr_token;


    void get_next();
    void expect(TokenType type, const std::string& error_msg);
    void S();             
    void CREATE_STMT();  
    void SELECT_STMT();   
    void INSERT_STMT();   
    void COLUMN_LIST();   
    void WHERE_CLAUSE();  

public:
    Parser(Scanner &sc) : scanner(sc) {}
    void analyze();      
};

#endif
