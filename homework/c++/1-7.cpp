class A {
    int val;
    static int count;
public:
    A(int n = 0) {
        ++count;
        if (count <= 2) val = n;
        else val = 7;
    }

    A& operator*=(int x) {
        val *= x;
        return *this;
    }

    A& operator*=(const A& other) {
        val *= other.val;
        return *this;
    }

    int get() const {
        return val;
    }
};

int A::count = 0;
