#include "parser.h"
#include <iostream>

void Parser::get_next() {
    curr_token = scanner->get_token(); // Обращение через указатель[cite: 4]
    if (curr_token.type != LEX_FINISH) {
        std::cout << " [Lex] Token: " << curr_token.value
                  << " (Type: " << curr_token.type << ")" << std::endl;
    }
}

void Parser::expect(TokenType type, const std::string& error_msg) {
    if (curr_token.type != type) {
        throw ParserException("Syntax Error: " + error_msg + " (expected " + std::to_string(type) + ", got " + curr_token.value + ")");
    }
    get_next();
}

void Parser::analyze() {
    get_next(); 
    S();
    if (curr_token.type != LEX_FINISH) {
        throw ParserException("Syntax Error: Unexpected tokens after semicolon");
    }
    std::cout << "--- Syntax & Semantic OK ---" << std::endl;
}

void Parser::S() {
    if (curr_token.type == LEX_SELECT) {
        SELECT_STMT();
    } else if (curr_token.type == LEX_CREATE) {
        CREATE_STMT();
    } else if (curr_token.type == LEX_INSERT) {
        INSERT_STMT();
    } else {
        throw ParserException("Syntax Error: Unknown command");
    }
    expect(LEX_SEMICOLON, "Missing semicolon");
}

void Parser::SELECT_STMT() {
    get_next(); 
    bool select_star = false;
    current_select_columns.clear();

    if (curr_token.type == LEX_STAR) {
        select_star = true;
        get_next();
    } else {
        COLUMN_LIST();
    }

    expect(LEX_FROM, "Expected FROM");
    std::string table_name = curr_token.value;
    expect(LEX_ID, "Expected table name");

    // СЕМАНТИКА: Проверка таблицы[cite: 4]
    if (tables.find(table_name) == tables.end()) {
        throw ParserException("Semantic Error: Table '" + table_name + "' does not exist.");
    }

    // СЕМАНТИКА: Проверка столбцов[cite: 4]
    if (!select_star) {
        for (const auto& col_name : current_select_columns) {
            bool found = false;
            for (const auto& def : tables[table_name].columns) {
                if (def.name == col_name) { found = true; break; }
            }
            if (!found) throw ParserException("Semantic Error: Column '" + col_name + "' not found.");
        }
    }

    if (curr_token.type == LEX_WHERE) WHERE_CLAUSE();
}

void Parser::CREATE_STMT() {
    get_next(); 
    expect(LEX_TABLE, "Expected TABLE");
    std::string t_name = curr_token.value;
    expect(LEX_ID, "Expected table name");

    if (tables.find(t_name) != tables.end()) {
        throw ParserException("Semantic Error: Table already exists.");
    }

    expect(LEX_LEFT_PAREN, "Expected '('");
    TableDef new_table;
    new_table.name = t_name;

    while (true) {
        std::string c_name = curr_token.value;
        expect(LEX_ID, "Expected column name");
        TokenType c_type = curr_token.type;
        if (c_type != LEX_TEXT && c_type != LEX_LONG) throw ParserException("Expected type");
        new_table.columns.push_back({c_name, c_type});
        get_next();
        if (curr_token.type == LEX_COMMA) get_next();
        else break;
    }
    expect(LEX_RIGHT_PAREN, "Expected ')'");
    tables[t_name] = new_table; // Сохранение в память[cite: 4, 5]
    std::cout << " [Semantic] Table " << t_name << " registered." << std::endl;
}

void Parser::COLUMN_LIST() {
    current_select_columns.push_back(curr_token.value);
    expect(LEX_ID, "Expected column name");
    if (curr_token.type == LEX_AS) { get_next(); expect(LEX_ID, "Alias expected"); }
    while (curr_token.type == LEX_COMMA) {
        get_next();
        current_select_columns.push_back(curr_token.value);
        expect(LEX_ID, "Expected column name");
        if (curr_token.type == LEX_AS) { get_next(); expect(LEX_ID, "Alias expected"); }
    }
}

void Parser::WHERE_CLAUSE() {
    get_next(); 
    while (curr_token.type != LEX_SEMICOLON && curr_token.type != LEX_FINISH) get_next();
}

void Parser::INSERT_STMT() {
    get_next();
    expect(LEX_INTO, "Expected INTO");
    std::string t_name = curr_token.value;
    expect(LEX_ID, "Expected table name");
    if (tables.find(t_name) == tables.end()) throw ParserException("Table not found");
    expect(LEX_VALUES, "Expected VALUES");
    expect(LEX_LEFT_PAREN, "Expected '('");
    while (curr_token.type != LEX_RIGHT_PAREN && curr_token.type != LEX_FINISH) get_next();
    expect(LEX_RIGHT_PAREN, "Expected ')'");
}
