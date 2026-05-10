#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <stack>

//описание столбца
struct ColumnDef {
    std::string name;
    TokenType type; 
};

//опсиание таблицы


struct TableDef {
    std::string name;
    std::vector<ColumnDef> columns;

    std::vector<std::vector<Token>> rows;
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
    std::vector<Token> poliz; //контейнер

    void get_next();
    void expect(TokenType type, const std::string& error_msg);
    void S();             
    void CREATE_STMT();  
    void SELECT_STMT();   
    void INSERT_STMT();   
    void COLUMN_LIST();

 //для полиза   
    void WHERE_CLAUSE();
    void OR_EXPR();
    void AND_EXPR();
    void NOT_EXPR();
    void RELATION();

    void SHOW_STMT();
    void DESCRIBE_STMT();

  //новые функции
    void push_poliz(Token t) {poliz.push_back(t);}
    void print_poliz();  
public:
    Parser() : scanner(nullptr) {} 
    void set_scanner(Scanner &sc) { scanner = &sc; } //обновление сканера
    void analyze();      
    bool evaluate_poliz(const std::vector<Token>& row, const TableDef& table);
    Token get_value_from_row(const std::string& col_name, const std::vector<Token>& row, const TableDef& table);
    void execute_operator(std::stack<Token>& st, TokenType op);
       
};

#endif
