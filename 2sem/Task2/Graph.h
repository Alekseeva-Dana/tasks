#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <cstring>

class Graph {
private:
    struct Vertex {
        char* name;
        Vertex* next;
        
        Vertex(const char* n) {
            name = new char[strlen(n) + 1];
            strcpy(name, n);
            next = nullptr;
        }
        
        ~Vertex() {
            delete[] name;
        }
    };
    
    struct Edge {
        char* from;
        char* to;
        char* label;
        Edge* next;
        
        Edge(const char* f, const char* t, const char* l) {
            from = new char[strlen(f) + 1];
            to = new char[strlen(t) + 1];
            label = new char[strlen(l) + 1];
            strcpy(from, f);
            strcpy(to, t);
            strcpy(label, l);
            next = nullptr;
        }
        
        ~Edge() {
            delete[] from;
            delete[] to;
            delete[] label;
        }
    };
    
    Vertex* vertices;
    Edge* edges;
    int vertexCount;
    int edgeCount;
    
    Vertex* findVertex(const char* name) {
        Vertex* curr = vertices;
        while (curr) {
            if (strcmp(curr->name, name) == 0) return curr;
            curr = curr->next;
        }
        return nullptr;
    }
    
    Edge* findEdge(const char* from, const char* to, const char* label) {
        Edge* curr = edges;
        while (curr) {
            if (strcmp(curr->from, from) == 0 && 
                strcmp(curr->to, to) == 0 &&
                strcmp(curr->label, label) == 0) return curr;
            curr = curr->next;
        }
        return nullptr;
    }
    
    void clear() {
        while (vertices) {
            Vertex* temp = vertices;
            vertices = vertices->next;
            delete temp;
        }
        
        while (edges) {
            Edge* temp = edges;
            edges = edges->next;
            delete temp;
        }
        
        vertexCount = 0;
        edgeCount = 0;
    }
    
    void copyFrom(const Graph& other) {
        clear();
        
        /* копируем вершины*/
        Vertex* otherV = other.vertices;
        while (otherV) {
            addVertex(otherV->name);
            otherV = otherV->next;
        }
        
        /* копируем ребра */
        Edge* otherE = other.edges;
        while (otherE) {
            addEdge(otherE->from, otherE->to, otherE->label);
            otherE = otherE->next;
        }
    }
    
    /* проверка достижимости */
    bool isReachableDFS(const char* current, const char* target, bool visited[], int maxVertices) const {
        if (strcmp(current, target) == 0) return true;
        
        Edge* e = edges;
        while (e) {
            if (strcmp(e->from, current) == 0) {
            }
            e = e->next;
        }
        
        return false;
    }
    
public:
    
    class Error {
    private:
        char message[256];
        int lineNum;
    public:
        Error(const char* msg, int line) {
            strncpy(message, msg, 255);
            message[255] = '\0';
            lineNum = line;
        }
        
        void print() const {
            std::cerr << "error at line " << lineNum << ": " << message << std::endl;
        }
    };
    
    /* конструкторы */
    Graph() : vertices(nullptr), edges(nullptr), vertexCount(0), edgeCount(0) {}
    
    Graph(const char* str) : vertices(nullptr), edges(nullptr), vertexCount(0), edgeCount(0) {
    
        if (!str) return;
        
        const char* p = str;
        char buffer[256];
        
       
        while (*p) {
            int i = 0;
            while (*p && *p != ',' && *p != ')' && *p != '(') {
                if (*p >= '0' && *p <= '9') {
                    buffer[i++] = *p;
                }
                p++;
            }
            if (i > 0) {
                buffer[i] = '\0';
                addVertex(buffer);
            }
            if (*p) p++;
        }
        
      
      //  if (vertexCount >= 3) {
        //    addEdge("1", "2", "a");
          //  addEdge("2", "3", "abc");
       // }
    }
    
    Graph(const Graph& other) : vertices(nullptr), edges(nullptr), vertexCount(0), edgeCount(0) {
        copyFrom(other);
    }
    
    ~Graph() {
        clear();
    }
    
    Graph& operator=(const Graph& other) {
        if (this != &other) {
            copyFrom(other);
        }
        return *this;
    }
    
    /* добавление вершин и ребер */
    void addVertex(const char* name) {
        if (!name || findVertex(name)) return;
        
        Vertex* v = new Vertex(name);
        v->next = vertices;
        vertices = v;
        vertexCount++;
    }
    
    void addEdge(const char* from, const char* to, const char* label = "") {
        if (!from || !to) return;
        
        /* добавляем вершины, если их нет */
        if (!findVertex(from)) addVertex(from);
        if (!findVertex(to)) addVertex(to);
        
        /* проверяем, есть ли уже такое ребро */
        if (findEdge(from, to, label)) return;
        
        Edge* e = new Edge(from, to, label);
        e->next = edges;
        edges = e;
        edgeCount++;
    }
    
    /* оператор + для добавления вершины */
    Graph& operator+(const char* vertex) {
        addVertex(vertex);
        return *this;
    }
    
    /* оператор + для объединения графов */
    Graph& operator+(const Graph& other) {
       
        Vertex* otherV = other.vertices;
        while (otherV) {
            addVertex(otherV->name);
            otherV = otherV->next;
        }
        
        /* копируем ребра*/
        Edge* otherE = other.edges;
        while (otherE) {
            addEdge(otherE->from, otherE->to, otherE->label);
            otherE = otherE->next;
        }
        
        return *this;
    }
    
    /* удаление вершины*/
    Graph& operator-(const char* vertex) {
        if (!vertex) return *this;
        
     
        Edge* curr = edges;
        Edge* prev = nullptr;
        while (curr) {
            if (strcmp(curr->from, vertex) == 0 || strcmp(curr->to, vertex) == 0) {
                Edge* toDel = curr;
                if (prev) {
                    prev->next = curr->next;
                    curr = curr->next;
                } else {
                    edges = curr->next;
                    curr = edges;
                }
                delete toDel;
                edgeCount--;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
        
     
        Vertex* vCurr = vertices;
        Vertex* vPrev = nullptr;
        while (vCurr) {
            if (strcmp(vCurr->name, vertex) == 0) {
                if (vPrev) {
                    vPrev->next = vCurr->next;
                } else {
                    vertices = vCurr->next;
                }
                delete vCurr;
                vertexCount--;
                break;
            }
            vPrev = vCurr;
            vCurr = vCurr->next;
        }
        
        return *this;
    }
    
    /* удаление ребра */
    void removeEdge(const char* from, const char* to, const char* label = "") {
        Edge* curr = edges;
        Edge* prev = nullptr;
        
        while (curr) {
            if (strcmp(curr->from, from) == 0 && 
                strcmp(curr->to, to) == 0 &&
                strcmp(curr->label, label) == 0) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    edges = curr->next;
                }
                delete curr;
                edgeCount--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    

    void operator delete[](void* ptr, const char* vertex) {
        if (ptr && vertex) {
            Graph* g = static_cast<Graph*>(ptr);
            g->removeVertex(vertex);
        }
    }
    
   
    void removeVertex(const char* name) {
        *this - name;
    }
    
    /* проверка достижимости */
    bool isReachable(const char* from, const char* to) const {
        if (!from || !to) return false;
        if (strcmp(from, to) == 0) return true;
        
      
        const int MAX_VERTICES = 100;
        const char* vertexNames[MAX_VERTICES];
        int vCount = 0;
        
        
        Vertex* v = vertices;
        while (v && vCount < MAX_VERTICES) {
            vertexNames[vCount++] = v->name;
            v = v->next;
        }
        
      
        bool* visited = new bool[vCount];
        for (int i = 0; i < vCount; i++) {
            visited[i] = false;
        }
        
       
        int startIndex = -1;
        for (int i = 0; i < vCount; i++) {
            if (strcmp(vertexNames[i], from) == 0) {
                startIndex = i;
                break;
            }
        }
        
        if (startIndex == -1) {
            delete[] visited;
            return false;
        }
        
       
        int stack[MAX_VERTICES];
        int top = 0;
        
        stack[top++] = startIndex;
        visited[startIndex] = true;
        
        while (top > 0) {
            int currentIndex = stack[--top];
            const char* current = vertexNames[currentIndex];
            
            
            Edge* e = edges;
            while (e) {
                if (strcmp(e->from, current) == 0) {
                   
                    int toIndex = -1;
                    for (int i = 0; i < vCount; i++) {
                        if (strcmp(vertexNames[i], e->to) == 0) {
                            toIndex = i;
                            break;
                        }
                    }
                    
                    if (toIndex != -1) {
                        if (strcmp(e->to, to) == 0) {
                            delete[] visited;
                            return true;
                        }
                        
                        if (!visited[toIndex]) {
                            visited[toIndex] = true;
                            if (top < MAX_VERTICES) {
                                stack[top++] = toIndex;
                            }
                        }
                    }
                }
                e = e->next;
            }
        }
        
        delete[] visited;
        return false;
    }
    
    /*поиск сильно связных компонент*/
    void findStronglyConnectedComponents() const {
        std::cout << "strongly connected components:" << std::endl;
        
     
        const int MAX = 100;
        const char* verts[MAX];
        int count = 0;
        
        Vertex* v = vertices;
        while (v && count < MAX) {
            verts[count++] = v->name;
            v = v->next;
        }
        
        if (count == 0) {
            std::cout << "  no vertices" << std::endl;
            return;
        }
        
      
        bool* visited = new bool[count];
        for (int i = 0; i < count; i++) visited[i] = false;
        
        
        for (int i = 0; i < count; i++) {
            if (!visited[i]) {
                std::cout << "  component: " << verts[i];
                visited[i] = true;
                
                for (int j = i + 1; j < count; j++) {
                    if (!visited[j]) {
                        if (isReachable(verts[i], verts[j]) && 
                            isReachable(verts[j], verts[i])) {
                            std::cout << ", " << verts[j];
                            visited[j] = true;
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
        
        delete[] visited;
    }
    
    /* вывод исходящих дуг из вершины */
    void printOutgoingEdges(const char* vertex) const {
        std::cout << "outgoing edges from " << vertex << ":" << std::endl;
        
        Edge* e = edges;
        bool found = false;
        while (e) {
            if (strcmp(e->from, vertex) == 0) {
                std::cout << "  -> " << e->to;
                if (strlen(e->label) > 0) {
                    std::cout << " [" << e->label << "]";
                }
                std::cout << std::endl;
                found = true;
            }
            e = e->next;
        }
        
        if (!found) {
            std::cout << "  no outgoing edges" << std::endl;
        }
    }
    
    /* проверка наличия */
    bool contains(const char* name) const {
        Vertex* v = vertices;
        while (v) {
            if (strcmp(v->name, name) == 0) return true;
            v = v->next;
        }
        return false;
    }
    
    bool contains(const char* from, const char* to, const char* label) const {
        Edge* e = edges;
        while (e) {
            if (strcmp(e->from, from) == 0 && 
                strcmp(e->to, to) == 0 &&
                strcmp(e->label, label) == 0) return true;
            e = e->next;
        }
        return false;
    }
    
    /* геттеры*/
    int getVertexCount() const { return vertexCount; }
    int getEdgeCount() const { return edgeCount; }
    
   
    friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
        os << "graph (" << g.vertexCount << " vertices, " << g.edgeCount << " edges)" << std::endl;
        
        os << "vertices: ";
        
    
        const int MAX = 100;
        const char* verts[MAX];
        int count = 0;
        
        Vertex* v = g.vertices;
        while (v && count < MAX) {
            verts[count++] = v->name;
            v = v->next;
        }
        
       
        for (int i = count - 1; i >= 0; i--) {
            os << verts[i];
            if (i > 0) os << ", ";
        }
        os << std::endl;
        
        os << "edges:" << std::endl;
        
      
        const int MAX_EDGES = 100;
        struct EdgeInfo {
            const char* from;
            const char* to;
            const char* label;
        };
        
        EdgeInfo edgeInfos[MAX_EDGES];
        int eCount = 0;
        
        Edge* e = g.edges;
        while (e && eCount < MAX_EDGES) {
            edgeInfos[eCount].from = e->from;
            edgeInfos[eCount].to = e->to;
            edgeInfos[eCount].label = e->label;
            eCount++;
            e = e->next;
        }
        
      
        for (int i = eCount - 1; i >= 0; i--) {
            os << "  " << edgeInfos[i].from << " -> " << edgeInfos[i].to;
            if (strlen(edgeInfos[i].label) > 0) {
                os << " [" << edgeInfos[i].label << "]";
            }
            os << std::endl;
        }
        
        return os;
    }
};

#endif
