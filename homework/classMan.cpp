#include <iostream>
#include <string>
using namespace std;

class Man {
    int age;
    string name;
public:
    Man(int age, string name) : age(age), name(name) {}
    virtual ~Man() {}
    virtual void print() const = 0;

protected:
    void printBase() const {
        cout << "Меня зовут " << name << ". Мне " << age << ". ";
    }
};

class Male : virtual public Man {
public:
    Male(int age = 29, string name = "Коля") : Man(age, name) {}
    virtual void print() const override {
        printBase();
        cout << "Я мужчина. ";
    }
};

class Female : virtual public Man {
public:
    Female(int age = 29, string name = "Оля") : Man(age, name) {}
    virtual void print() const override {
        printBase();
        cout << "Я женщина. ";
    }
};
class LovesSports {
public:
    virtual void printHobby() const {
        cout << "Я люблю спорт. ";
    }
};

class LovesKvass {
public:
    virtual void printHobby() const {
        cout << "Я люблю квас. ";
    }
};

class LovesJewelry {
public:
    virtual void printHobby() const {
        cout << "Я люблю украшения. ";
    }
};

class LovesScience {
public:
    virtual void printHobby() const {
        cout << "Я люблю читать научные труды. ";
    }
};


class Sportsman : public Male, public LovesSports {
public:
    Sportsman(int age = 19, string name = "Дима") : Man(age, name), Male(age, name) {}
    virtual void print() const override {
        Male::print();
        LovesSports::printHobby();
    }
};

class Funny_guy : public Male, public LovesKvass {
public:
    Funny_guy(int age = 19, string name = "Саша") : Man(age, name), Male(age, name) {}
    virtual void print() const override {
        Male::print();
        LovesKvass::printHobby();
    }
};

class Cover_girl : public Female, public LovesJewelry {
public:
    Cover_girl(int age = 19, string name = "Даша") : Man(age, name), Female(age, name) {}
    virtual void print() const override {
        Female::print();
        LovesJewelry::printHobby();
    }
};

class Clever_girl : public Female, public LovesScience {
public:
    Clever_girl(int age = 19, string name = "Нина") : Man(age, name), Female(age, name) {}
    virtual void print() const override {
        Female::print();
        LovesScience::printHobby();
    }
};

class SportyKvassLover : public Male, public LovesSports, public LovesKvass {
public:
    SportyKvassLover(int age = 20, string name = "Петя") 
        : Man(age, name), Male(age, name) {}
    
    virtual void print() const override {
        Male::print();
        LovesSports::printHobby();
        LovesKvass::printHobby();
    }
};


class IntellectualWithStyle : public Female, public LovesJewelry, public LovesScience {
public:
    IntellectualWithStyle(int age = 24, string name = "Аня")
        : Man(age, name), Female(age, name) {}
    
    virtual void print() const override {
        Female::print();
        LovesJewelry::printHobby();
        LovesScience::printHobby();
    }
};


class SuperMan : public Male, public LovesSports, public LovesKvass, public LovesScience {
public:
    SuperMan(int age = 30, string name = "Супермен")
        : Man(age, name), Male(age, name) {}
    
    virtual void print() const override {
        Male::print();
        cout << "Я уникален! ";
        LovesSports::printHobby();
        LovesKvass::printHobby();
        LovesScience::printHobby();
    }
};

// Женщина с тремя увлечениями
class SuperWoman : public Female, public LovesJewelry, public LovesScience, public LovesSports {
public:
    SuperWoman(int age = 28, string name = "Чудо-женщина")
        : Man(age, name), Female(age, name) {}
    
    virtual void print() const override {
        Female::print();
        LovesJewelry::printHobby();
        LovesScience::printHobby();
        LovesSports::printHobby();
    }
};


void People(Man* p[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "Person #" << i + 1 << ":" << endl;
        p[i]->print();
        cout << endl << endl;
    }
}


int main() {
    
    Clever_girl c(18, "Нина");
    Female f(29, "Оля");
    Sportsman s(25, "Дима");
    Funny_guy p(19, "Саша");
    Male m(45, "Миша");
    Cover_girl cg(25, "Кристина");
    
    // Новые классы с комбинацией увлечений
    SportyKvassLover skv(22, "Витя");
    IntellectualWithStyle iws(24, "Аня");
    SuperMan sm(35, "Арнольд");
    SuperWoman sw(27, "Елена");
    
    
    Man* arr[] = { 
        &c, &f, &s, &p, &m, &cg, 
        &skv, &iws, &sm, &sw 
    };
    
    return 0;
}
