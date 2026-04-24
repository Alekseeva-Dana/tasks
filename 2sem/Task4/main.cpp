#include <iostream>
#include <string>
#include "scanner.h"
#include "parser.h"

int main() {
    std::string line;
    std::cout << "Model SQL Interpreter (Lexical + Syntactic Mode)" << std::endl;
    std::cout << "Type 'EXIT' to quit." << std::endl;

    while (true) {
        std::cout << "sql> ";
        if (!std::getline(std::cin, line) || line == "EXIT") break;
        if (line.empty()) continue;

        try {
         
            
            Scanner scanner(line);
            Parser parser(scanner);
            
            parser.analyze();
            
            

        } catch (const ScannerException& e) {
            std::cerr << "LEXICAL ERROR: " << e.what() << std::endl;
        } catch (const ParserException& e) {
            std::cerr << "SYNTAX ERROR: " << e.what() << std::endl;
        }
    }
    return 0;
}
