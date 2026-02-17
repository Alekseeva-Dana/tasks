class C {
    int val;
public:
    C(int n) : val(n * 2) {} 
    C operator+(const C& other) const {
        return C(val + other.val); 
    }
    int get() const { return val; }
};
