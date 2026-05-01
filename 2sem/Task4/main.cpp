#include <iostream>
#include <string>
#include "scanner.h"
#include "parser.h"

int main() {
    std::string line;
    std::cout << "Model SQL Interpreter (Persistent Mode)" << std::endl;
    
    Parser parser; //создаем один раз!

    while (true) {
        std::cout << "sql> ";
        if (!std::getline(std::cin, line) || line == "EXIT") break;
        if (line.empty()) continue;

        try {
            Scanner scanner(line); //сканеры
            parser.set_scanner(scanner); //привязка
            parser.analyze();
        } catch (const ScannerException& e) {
            std::cerr << "LEXICAL ERROR: " << e.what() << std::endl;
        } catch (const ParserException& e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
        }
    }
    return 0;
}
