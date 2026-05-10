#include "parser.h"
#include <iostream>
#include <stack>

void Parser::get_next() {
    curr_token = scanner->get_token(); 
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
    poliz.clear();
    get_next(); 
    S();
    if (curr_token.type != LEX_FINISH) {
        throw ParserException("Syntax Error: Unexpected tokens after semicolon");
    }
    std::cout << "--- Syntax & Semantic OK ---" << std::endl;
    print_poliz();
}

void Parser::print_poliz() {
	if (poliz.empty()) return;
	std::cout << " [POLIZ]: ";
	for (const auto& t : poliz) {
		std::cout << t.value << " ";
	}
	std::cout << std::endl;
}

void Parser::S() {
    if (curr_token.type == LEX_SELECT) {
        SELECT_STMT();
    } else if (curr_token.type == LEX_CREATE) {
        CREATE_STMT();
    } else if (curr_token.type == LEX_INSERT) {
        INSERT_STMT();
    } else if (curr_token.type == LEX_SHOW) {
	SHOW_STMT();
    } else if (curr_token.type == LEX_DESCRIBE) {
	DESCRIBE_STMT();
    } else {
        throw ParserException("Syntax Error: Unknown command");
    }
    expect(LEX_SEMICOLON, "Missing semicolon");
}


//методы для полиза
void Parser::WHERE_CLAUSE() {
	get_next();
	OR_EXPR();
}

void Parser::OR_EXPR() {
	AND_EXPR();
	while (curr_token.type == LEX_OR) {
		Token op = curr_token;
		get_next();
		AND_EXPR();
		push_poliz(op);
	}
}

void Parser::AND_EXPR() {
	NOT_EXPR();
	while (curr_token.type == LEX_AND) {
		Token op = curr_token;
		get_next();
		NOT_EXPR();
		push_poliz(op);
	}
}

void Parser::NOT_EXPR() {
	if (curr_token.type == LEX_NOT) {
		Token op = curr_token;
		get_next();
		NOT_EXPR();
		push_poliz(op);
	} else {
		RELATION();
	}
}

bool Parser::evaluate_poliz(const std::vector<Token>& row, const TableDef& table) {
    std::stack<Token> st; 

    for (const auto& poliz_token : poliz) {
        if (poliz_token.type == LEX_NUM || poliz_token.type == LEX_STR) {
            st.push(poliz_token);
        } 
        else if (poliz_token.type == LEX_ID) {
         
            st.push(get_value_from_row(poliz_token.value, row, table));
        } 
        else {
          
            execute_operator(st, poliz_token.type);
        }
    }
  
    return st.top().int_value != 0;
}

void Parser::RELATION() {
   
    if (curr_token.type == LEX_LEFT_PAREN) {
        get_next(); 
        OR_EXPR(); 
        expect(LEX_RIGHT_PAREN, "Missing ')'");
    } 
    else {
       
        Token t1 = curr_token; 
        if (t1.type != LEX_ID && t1.type != LEX_NUM && t1.type != LEX_STR) {
            throw ParserException("Expected operand (ID, Number or String)");
        }
        get_next();

       
        if (curr_token.type == LEX_EQ  || curr_token.type == LEX_LSS  ||
            curr_token.type == LEX_GTR  || curr_token.type == LEX_LEQ ||  
            curr_token.type == LEX_GEQ || curr_token.type == LEX_NEQ) {
            
           
            Token op = curr_token; 
            get_next();

          
            Token t2 = curr_token; 
            if (t2.type != LEX_ID && t2.type != LEX_NUM && t2.type != LEX_STR) {
                throw ParserException("Expected second operand after operator");
            }
            get_next();

            //делаем полиз
            push_poliz(t1);
            push_poliz(t2);
            push_poliz(op); 
           
        } 
        else {
        
            push_poliz(t1);
        }
    }
}

void Parser::SELECT_STMT() {
    get_next(); 
    bool select_all = false;
    if (curr_token.type == LEX_STAR) {
        select_all = true;
        get_next();
    } else {
        COLUMN_LIST();
    }

    expect(LEX_FROM, "Expected FROM");
    std::string t_name = curr_token.value;
    expect(LEX_ID, "Expected table name");

    if (tables.find(t_name) == tables.end()) {
        throw ParserException("Semantic Error: Table not found");
    }

    
    poliz.clear();
    if (curr_token.type == LEX_WHERE) {
        WHERE_CLAUSE();
    }

    //выполнение exec
    TableDef& table = tables[t_name];
    std::cout << "\n--- Result Table: " << t_name << " ---" << std::endl;

 
    for (auto& col : table.columns) std::cout << col.name << "\t";
    std::cout << "\n-------------------------------" << std::endl;

    int count = 0;
    for (const auto& row : table.rows) {
      
        if (poliz.empty() || evaluate_poliz(row, table)) {
            for (const auto& cell : row) {
                if (cell.type == LEX_NUM) std::cout << cell.int_value << "\t";
                else std::cout << cell.value << "\t";
            }
            std::cout << std::endl;
            count++;
        }
    }
    std::cout << "-------------------------------\n Rows found: " << count << std::endl;
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
    tables[t_name] = new_table; 
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


void Parser::INSERT_STMT() {
    get_next(); 
    expect(LEX_INTO, "Expected INTO");
    
    std::string t_name = curr_token.value;
    expect(LEX_ID, "Expected table name");

   
    if (tables.find(t_name) == tables.end()) {
        throw ParserException("Semantic Error: Table '" + t_name + "' not found.");
    }

    expect(LEX_VALUES, "Expected VALUES");
    expect(LEX_LEFT_PAREN, "Expected '('");

    TableDef& table = tables[t_name];
    std::vector<Token> new_row;
    size_t col_index = 0;

    
    while (true) {
        if (col_index >= table.columns.size()) {
            throw ParserException("Semantic Error: Too many values for table " + t_name);
        }

      
        if (curr_token.type != LEX_NUM && curr_token.type != LEX_STR) {
            throw ParserException("Expected constant value in VALUES");
        }

        new_row.push_back(curr_token);
        get_next();

        if (curr_token.type == LEX_COMMA) {
            get_next();
            col_index++;
        } else break;
    }

    expect(LEX_RIGHT_PAREN, "Expected ')'");

    if (new_row.size() != table.columns.size()) {
        throw ParserException("Semantic Error: Not enough values");
    }

   
    table.rows.push_back(new_row);
    std::cout << " [Exec] 1 row inserted into " << t_name << std::endl;
}

Token Parser::get_value_from_row(const std::string& col_name, const std::vector<Token>& row, const TableDef& table) {
    for (size_t i = 0; i < table.columns.size(); ++i) {
        if (table.columns[i].name == col_name) {
            return row[i];
        }
    }
    throw ParserException("Semantic Error: Column '" + col_name + "' not found");
}

void Parser::SHOW_STMT() {
	get_next();
	expect(LEX_TABLES, "Expected TABLES after SHOW");
	std::cout << "Registered Tables" << std::endl;
	if (tables.empty()) {
		std::cout << "(no tables created)" << std::endl;
	} else {
		for (auto const& [name, def] : tables) {
			std::cout << "->" << name << std::endl;
		}
	}
	std::cout << "-------------------------" << std::endl;
}

void Parser::DESCRIBE_STMT() {
	get_next();
	std::string t_name = curr_token.value;
	expect(LEX_ID, "Expected table name");

	if (tables.find(t_name) == tables.end()) {
		throw ParserException("Semantic Error: Table ' " + t_name + " ' does not exist");
	}

	std::cout << "Structure of " << t_name << std::endl;
	for (const auto& col: tables[t_name].columns) {
		std::string type_name = (col.type == LEX_LONG) ? "LONG" : "TEXT";
		std::cout << "Column: " << col.name << " | Type: " << type_name << std::endl;
	}
	std::cout << "-------------------------" << std::endl;
}

void Parser::execute_operator(std::stack<Token>& st, TokenType op) {
    Token res;
  
    Token arg2 = st.top();
    st.pop();

    
    if (op == LEX_NOT) {
        res = Token(LEX_NUM, "");
        res.int_value = (arg2.int_value == 0 ? 1 : 0);
    } else {
      
        if (st.empty()) throw ParserException("Exec Error: Stack is empty, but binary operator found");

        Token arg1 = st.top();
        st.pop();

        res = Token(LEX_NUM, "");

        switch(op) {
            case LEX_GTR: //больше
                res.int_value = (arg1.int_value > arg2.int_value);
                break;
            case LEX_LSS: //меньше
                res.int_value = (arg1.int_value < arg2.int_value);
                break;
            case LEX_EQ:  //равно
                if (arg1.type == LEX_STR) res.int_value = (arg1.value == arg2.value);
                else res.int_value = (arg1.int_value == arg2.int_value);
                break;
            case LEX_NEQ: //не равно
                if (arg1.type == LEX_STR) res.int_value = (arg1.value != arg2.value);
                else res.int_value = (arg1.int_value != arg2.int_value);
                break;
            case LEX_AND: //и
                res.int_value = (arg1.int_value && arg2.int_value);
                break;
            case LEX_OR:  //или
                res.int_value = (arg1.int_value || arg2.int_value);
                break;
            case LEX_LEQ: //<=
                res.int_value = (arg1.int_value <= arg2.int_value);
                break;
            case LEX_GEQ: //>=
                res.int_value = (arg1.int_value >= arg2.int_value);
                break;
            default:
                throw ParserException("Exec Error: Unknown operator type in POLIZ");
        }
    }

    
    res.type = LEX_NUM;
    st.push(res);
}
