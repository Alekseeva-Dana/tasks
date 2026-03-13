#include "Graph.h"
#include <iostream>
#include <cstring>

void runTests() {
    
    try {
        /* конструктор по умолчанию */
        std::cout << "\n--- default constructor ---" << std::endl;
        Graph g1;
        std::cout << "graph created" << std::endl;
        
        /* добавление вершин */
        std::cout << "\n--- adding vertices ---" << std::endl;
        g1 + "A" + "B" + "C";
        std::cout << "added vertices A, B, C" << std::endl;
        std::cout << g1 << std::endl;
        
        /* добавление ребер */
        std::cout << "\n--- adding edges ---" << std::endl;
        g1.addEdge("A", "B", "edge1");
        g1.addEdge("B", "C", "edge2");
        g1.addEdge("C", "A", "edge3");
        std::cout << "added edges" << std::endl;
        std::cout << g1 << std::endl;
        
        /* конструктор из строки */
        std::cout << "\n--- string constructor ---" << std::endl;
        Graph g2("(1,2,3,4)");
        std::cout << g2 << std::endl;
        
        /* конструктор копирования */
        std::cout << "\n--- copy constructor ---" << std::endl;
        Graph g3 = g1;
        std::cout << "copied graph:" << std::endl;
        std::cout << g3 << std::endl;
        
        /* оператор присваивания */
        std::cout << "\n--- assignment operator ---" << std::endl;
        Graph g4;
        g4 = g1;
        std::cout << "assigned graph:" << std::endl;
        std::cout << g4 << std::endl;
        
        /* объединение графов */
        std::cout << "\n--- graph union (operator+) ---" << std::endl;
        Graph g5;
        g5 + "X" + "Y";
        g5.addEdge("X", "Y", "xy");
        g1 + g5;
        std::cout << "after union with graph containing X,Y:" << std::endl;
        std::cout << g1 << std::endl;
        
        /* проверка достижимости */
        std::cout << "\n--- reachability ---" << std::endl;
        std::cout << "is A reachable from A? " << (g1.isReachable("A", "A") ? "yes" : "no") << std::endl;
        std::cout << "is C reachable from A? " << (g1.isReachable("A", "C") ? "yes" : "no") << std::endl;
        std::cout << "is X reachable from A? " << (g1.isReachable("A", "X") ? "yes" : "no") << std::endl;
        
        /* вывод исходящих дуг */
        std::cout << "\n--- outgoing edges ---" << std::endl;
        g1.printOutgoingEdges("A");
        
        /* удаление вершины */
        std::cout << "\n--- removing vertex ---" << std::endl;
        g1 - "B";
        std::cout << "after removing vertex B:" << std::endl;
        std::cout << g1 << std::endl;
        
        /* удаление ребра */
        std::cout << "\n--- removing edge ---" << std::endl;
        g1.removeEdge("A", "B", "edge1");
        std::cout << "after removing edge A->B:" << std::endl;
        std::cout << g1 << std::endl;
        
        /* проверка наличия */
        std::cout << "\n--- contains ---" << std::endl;
        std::cout << "contains vertex A? " << (g1.contains("A") ? "yes" : "no") << std::endl;
        std::cout << "contains vertex B? " << (g1.contains("B") ? "yes" : "no") << std::endl;
        std::cout << "contains edge C->A? " << (g1.contains("C", "A", "edge3") ? "yes" : "no") << std::endl;
        
        /* сильно связные компоненты */
        std::cout << "\n--- strongly connected components ---" << std::endl;
        Graph g6;
        g6 + "1" + "2" + "3" + "4";
        g6.addEdge("1", "2", "");
        g6.addEdge("2", "3", "");
        g6.addEdge("3", "1", "");
        g6.addEdge("3", "4", "");
        g6.addEdge("4", "3", "");
        std::cout << g6 << std::endl;
        g6.findStronglyConnectedComponents();
        
        
    } catch (const Graph::Error& e) {
        e.print();
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        runTests();
        return 0;
    }
    
    std::cout << "Simple Graph ADT Demo" << std::endl;
    std::cout << "=====================" << std::endl;
    
    Graph graph;
    std::string cmd;
    
    std::cout << "Commands: addV <name>, addE <from> <to> [label], removeV <name>, " << std::endl;
    std::cout << "          removeE <from> <to> [label], print, reach <from> <to>, " << std::endl;
    std::cout << "          outgoing <vertex>, components, quit" << std::endl;
    
    while (true) {
        std::cout << "\n> ";
        std::cin >> cmd;
        
        if (cmd == "quit") {
            break;
        }
        else if (cmd == "addV") {
            char name[100];
            std::cin >> name;
            graph + name;
            std::cout << "Vertex " << name << " added" << std::endl;
        }
        else if (cmd == "addE") {
            char from[100], to[100], label[100] = "";
            std::cin >> from >> to;
            if (std::cin.peek() != '\n') {
                std::cin >> label;
            }
            graph.addEdge(from, to, label);
            std::cout << "Edge " << from << "->" << to << " added" << std::endl;
        }
        else if (cmd == "removeV") {
            char name[100];
            std::cin >> name;
            graph - name;
            std::cout << "Vertex " << name << " removed" << std::endl;
        }
        else if (cmd == "removeE") {
            char from[100], to[100], label[100] = "";
            std::cin >> from >> to;
            if (std::cin.peek() != '\n') {
                std::cin >> label;
            }
            graph.removeEdge(from, to, label);
            std::cout << "Edge removed" << std::endl;
        }
        else if (cmd == "print") {
            std::cout << graph << std::endl;
        }
        else if (cmd == "reach") {
            char from[100], to[100];
            std::cin >> from >> to;
            std::cout << from << " -> " << to << " : " 
                      << (graph.isReachable(from, to) ? "reachable" : "not reachable") << std::endl;
        }
        else if (cmd == "outgoing") {
            char vertex[100];
            std::cin >> vertex;
            graph.printOutgoingEdges(vertex);
        }
        else if (cmd == "components") {
            graph.findStronglyConnectedComponents();
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }
    
    return 0;
}
