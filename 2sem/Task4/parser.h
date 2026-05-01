#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include <map>
#include <vector>
#include <string>
#include <stdexcept>

//описание столбца
struct ColumnDef {
    std::string name;
    TokenType type; 
};

//опсиание таблицы
struct TableDef {
    std::string name;
    std::vector<ColumnDef> columns;
};

class ParserException : public std::runtime_error {
public:
    ParserException(const std::string& msg) : std::runtime_error(msg) {}
};

class Parser {
    Scanner *scanner; 
    Token curr_token;

    //память парсера
    std::map<std::string, TableDef> tables; 
    std::vector<std::string> current_select_columns; 

    void get_next();
    void expect(TokenType type, const std::string& error_msg);
    void S();             
    void CREATE_STMT();  
    void SELECT_STMT();   
    void INSERT_STMT();   
    void COLUMN_LIST();   
    void WHERE_CLAUSE();  

public:
    Parser() : scanner(nullptr) {} 
    void set_scanner(Scanner &sc) { scanner = &sc; } //обновление сканера
    void analyze();      
};

#endif
