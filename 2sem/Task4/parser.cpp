#include "parser.h"
#include <iostream>

void Parser::get_next() {
    curr_token = scanner.get_token();

    if (curr_token.type != LEX_FINISH) {
	    std::cout << " [Lex] Token: " << curr_token.value
		    << " (Type: " << curr_token.type << ")" << std::endl;
    }
}

void Parser::expect(TokenType type, const std::string& error_msg) {
    if (curr_token.type != type) {
        throw ParserException(error_msg + " (expected " + std::to_string(type) + ", got " + curr_token.value + ")");
    }
    get_next();
}

void Parser::analyze() {
    get_next(); 
    S();
    if (curr_token.type != LEX_FINISH) {
        throw ParserException("Unexpected tokens after semicolon");
    }
    std::cout << "--- Syntax OK ---" << std::endl;
}

void Parser::S() {
    if (curr_token.type == LEX_SELECT) {
        SELECT_STMT();
    } else if (curr_token.type == LEX_CREATE) {
        CREATE_STMT();
    } else if (curr_token.type == LEX_INSERT) {
        INSERT_STMT();
    } else {
        throw ParserException("Unknown command. Must start with SELECT, CREATE or INSERT");
    }
    
    expect(LEX_SEMICOLON, "Missing semicolon at the end of command");
}


void Parser::SELECT_STMT() {
    get_next(); 

    if (curr_token.type == LEX_STAR) {
        get_next();
    } else {
        COLUMN_LIST();
    }

    expect(LEX_FROM, "Expected FROM after column list");
    expect(LEX_ID, "Expected table name after FROM");

    if (curr_token.type == LEX_WHERE) {
        WHERE_CLAUSE();
    }
}


void Parser::CREATE_STMT() {
    get_next(); 
    expect(LEX_TABLE, "Expected TABLE after CREATE");
    expect(LEX_ID, "Expected table name");
    expect(LEX_LEFT_PAREN, "Expected '(' after table name");

  
    while (true) {
        expect(LEX_ID, "Expected column name");
        if (curr_token.type != LEX_TEXT && curr_token.type != LEX_LONG) {
            throw ParserException("Expected type (TEXT or LONG) for column");
        }
        get_next();

        if (curr_token.type == LEX_COMMA) {
            get_next();
        } else break;
    }

    expect(LEX_RIGHT_PAREN, "Expected ')'");
}

void Parser::COLUMN_LIST() {
    expect(LEX_ID, "Expected column name");
    while (curr_token.type == LEX_COMMA) {
        get_next();
        expect(LEX_ID, "Expected column name after ','");
    }
}

void Parser::WHERE_CLAUSE() {
    get_next(); 
    while (curr_token.type != LEX_SEMICOLON && curr_token.type != LEX_FINISH) {
        get_next();
    }
}

void Parser::INSERT_STMT() {
    get_next();
    expect(LEX_INTO, "Expected INTO");
    expect(LEX_ID, "Expected table name");
    expect(LEX_VALUES, "Expected VALUES");
  
    expect(LEX_LEFT_PAREN, "Expected '('");
    while (curr_token.type != LEX_RIGHT_PAREN && curr_token.type != LEX_FINISH) {
        get_next();
    }
    expect(LEX_RIGHT_PAREN, "Expected ')'");
}
