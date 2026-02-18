Вот полный код с добавленными необходимыми методами:

```cpp
// Абстрактный тип данных "Список"
#include <iostream>     

using namespace std;

class List {
    // список реализуется цепочкой звеньев в динамич.памяти
    struct Node {    // тип "звено динамической цепочки"
       char elem;
       Node* next;
       
       // конструктор для звена, три в одном: умолчания, преобразования char --> Node, с двумя параметрами
       Node (char elem='_', Node* next=NULL) {
			this->elem=elem;
			this->next=next;
       }
    };
    Node* lst; // указатель на первое  звено цепочки
public:
    // конструктор умолчания
    List () :lst(NULL) {}

    // конструктор преобразования char --> List
    List (char c) {
        lst=new Node(c);
    }

    // конструктор преобразования "Си-строка" --> List
    List (const char* s) {
        if (*s=='\0') {
            lst=NULL;
            return;
        }
           
        Node *p;
        lst= p= new Node(*s++);
				
	while (*s!='\0') {
	    p= p->next= new Node(*s++);
	}
    }
    
    // проверка на пустоту списка
    bool empty() const {return lst==NULL;}

    // конструктор копирования
 	List (const List& l) {
		if (l.empty()) {
			lst=NULL;
			return;
		}
		Node *p, *pl=l.lst;
		p= lst= new Node(pl->elem);
		pl=pl->next; 
		while (pl!=NULL) {
			p->next= new Node(pl->elem);
			p=p->next;
			pl=pl->next;  
		}
	}
    
    // КОНСТРУКТОР ПЕРЕМЕЩЕНИЯ
    List (List&& l) noexcept : lst(l.lst) {
        l.lst = nullptr;
    }
      
    // очистка списка (с освобождением памяти)
    void clear() {
		Node *pl;
		while (lst!=NULL) {
			pl=lst;
			lst=lst->next;
			delete pl;
		}
	}

    // деструктор
	~List() {
		this->clear();
    }
		
    // операция +, конкатенация списков (метод класса)
	List operator+ (const List& l) const {
		Node *p,*pl;
	
        if(this->empty()){
               return l;
        }
        if(l.empty()){
               return *this;
        }
        
        List res(*this);
           
        p=res.lst;
        while(p->next!=NULL){
            p=p->next;
        }

        pl=l.lst;
		
        while (pl!=NULL) {
			p=p->next=new Node(pl->elem);
            pl=pl->next;
		}	
		
		return res;
	}
    
    // ОПЕРАЦИЯ L + 'a' (добавить в конец)
    List operator+(char c) const {
        List res(*this);
        if (res.lst) {
            Node* p = res.lst;
            while (p->next) p = p->next;
            p->next = new Node(c);
        } else {
            res.lst = new Node(c);
        }
        return res;
    }
    
    // ОПЕРАЦИЯ L - 'a' (удалить все вхождения символа)
    List operator-(char c) const {
        List res;
        Node** pp = &res.lst;
        Node* pl = lst;
        while (pl) {
            if (pl->elem != c) {
                *pp = new Node(pl->elem);
                pp = &((*pp)->next);
            }
            pl = pl->next;
        }
        return res;
    }

    // копирующее присваивание
	List& operator= (const List& l) {
        Node *p,*pl;
        
        //особый случай: l=l;
        if (&l==this){
                return *this;
        }
        
        //общий случай: разрушить старое, построить новое
        this->clear();

        if (l.lst==NULL) {
		 	return *this;
        }
        p=lst= new Node(l.lst->elem);
        pl=l.lst->next;
        while (pl!=NULL) {
		 	p->next= new Node(pl->elem);
		 	p=p->next;
		 	pl=pl->next;
        }
        
		return *this;
	}
    
    // ПРИСВАИВАНИЕ ПЕРЕМЕЩЕНИЕМ
    List& operator=(List&& l) noexcept {
        if (this != &l) {
            clear();
            lst = l.lst;
            l.lst = nullptr;
        }
        return *this;
    }

    // дружественная функция для печати списка (не член класса)
    friend ostream& operator<< (ostream& s, const List& l) {
		if (l.lst==NULL) {
			s<<"";
			return s;
		}
		Node *pl=l.lst;
		while (pl!=NULL) {  
			s<<pl->elem;
			pl=pl->next;

		}
		
		return s;
	}
	
    // ДРУЗЬЯ ДЛЯ СИММЕТРИЧНЫХ ОПЕРАЦИЙ
    friend List operator+(char c, const List& l);
    friend List operator-(char c, const List& l);
};

// ОПЕРАЦИЯ 'a' + L (добавить в начало)
List operator+(char c, const List& l) {
    List res;
    res.lst = new Node(c);
    if (l.lst) {
        Node* p = res.lst;
        Node* pl = l.lst;
        while (pl) {
            p->next = new Node(pl->elem);
            p = p->next;
            pl = pl->next;
        }
    }
    return res;
}

// ОПЕРАЦИЯ 'a' - L (оставить только символ a)
List operator-(char c, const List& l) {
    List res;
    Node** pp = &res.lst;
    Node* pl = l.lst;
    while (pl) {
        if (pl->elem == c) {
            *pp = new Node(c);
            pp = &((*pp)->next);
        }
        pl = pl->next;
    }
    return res;
}

int main() {
	List l1,l2('a'), l3("abc"), l4(l1), l5(l3);
	cout<<l1<<' '<<l2<<' '<<l3<<' '<<l4<<' '<<l5<<endl;
    //будет напечатано:_a_abc__abc
	cout<<(l1=l4)<<' '<<(l3)<<' ', cout<<(l4=l3)<<endl;
    //будет напечатано:_abc_abc
    cout<<l3+'a'; // добавить букву 'a' в конец списка, результат -- новый список
    //будет напечатано:abca
    
    // Реализовать, добавив подходящие операции (как метод класса или  как друг класса):
    cout<<l3-'a'; // удалить все  вхождения 'a'
    //должно быть напечатано:bc
    
    cout<< 'a'-List("abacaa"); // оставить в списке только буквы 'а'. Должно быть напечатано: aaaa
    cout<<'a'+l3;  // добавить  в начало списка букву 'a'
    //должно быть напечатано: aabc
	return 0;
}
```
