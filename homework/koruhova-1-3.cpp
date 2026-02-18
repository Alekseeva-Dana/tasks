class my_vector {
	int len;
	int *v;

public:
	//конструктор
	my_vector(int length, const int* arr = nullptr) : len(length) {
		v = new int[len];
		if (arr) {
			for (int i = 0; i < len; i++)
				v[i] = arr[i];
		} else {
			for (int i = 0; i < len; i++)
				v[i] = 0;
		}
	}

	//конструктор копирования
	my_vector(const my_vector& other) : len(other.len) {
		v = new int[len];
		for (int i = 0; i < len; i++)
			v[i] = other.v[i];
	}

	//деструктор
	~my_vector() {
		delete[] v;
	}

	//длина вектор
	int length() const {
		return len;
	}

	//скалярное произведение
	int dot(const my_vector& other) const {
		int sum = 0;
		for (int i = 0; i < len; i++)
			sum += v[i] * other.v[i];
		return sum;
	}

	//умножение на константу
	my_vector multiply(int scalar) const {
		my_vector result(len);
		for (int i = 0; i < len; i++)
			result.v[i] = v[i] * scalar;
		return result;
	}

	//сумма векторов
	my_vector add(const my_vector& other) const {
		my_vector result(len);
		for (int i = 0; i < len; i++)
			result.v[i] = v[i] + other.v[i];
		return result;
	}

	//функция next
	my_vector& next() {
		for (int i = 0; i < len; i++)
			v[i]++;
		return *this;
	}
};

//вектор увеличится дважды после вызова v.next().next()
