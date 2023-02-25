
#include <initializer_list>

template<typename C>
void inspect_container(C &c) {
	std::cout << "vector: " << &c << "\n";
	std::cout << "	size: " << c.size() << "\n";
	std::cout << "	cap: " << c.capacity() << "\n";
	for (auto &e : c) {
		std::cout << "	" << &e << " " << e << "\n";
	}
	std::cout << std::endl;
}

template<typename T, typename Alloc>
class Matrix {
public:
	void inspect() {
		std::cout << "Matrix: " << this << "\n" << "Shape: " << shape.first << 'x' << shape.second  << "\n";
		std::cout << "{";
		auto &[rows, columns] = shape;
		for (int y = 0; y < rows; ++y) {
			std::cout << (y == 0 ? " " : "  ");
			for (int x = 0; x < columns; ++x) {
				std::cout << c[x + y * columns] << ' ';
			}
			std::cout << (y == rows - 1 ? "" : "\n");
		}
		std::cout << "}\n\n";
	}

	void inspect_shape() {
		std::cout << shape.first << "x" << shape.second << "  ";
	}

private:
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_type;
	using container = std::vector<value_type, Alloc>;
	using initializer_list = std::initializer_list<value_type>;
	using pair = std::pair<size_type, size_type>;

	// TODO: create utility class for random values
	// std::random_device rnd{};
    // std::mersenne_twister_engine engine{rd()};
	// std::normal_distribution<value_type> gen(2, 5);

	pair shape;
	size_type size;
	container c;

public:
	Matrix() : shape({0, 0}), size{0}, c{} {};
	Matrix(initializer_list li, pair shape) : shape{shape}, size{shape.first * shape.second}, c{li} {}
	Matrix(pair shape) : shape{shape}, size{shape.first * shape.second}, c(size) {}

	Matrix(const Matrix&) = default;
	Matrix &operator=(const Matrix&) = default;
	Matrix(Matrix&&) noexcept = default;
	Matrix &operator=(Matrix&&) noexcept = default;

	~Matrix() {
		// inspect();
	};

	Matrix operator+(const Matrix &rhs) {
		if (shape != rhs.shape)
			throw std::invalid_argument("bad matrix shape");
		Matrix ret(*this);
		for (int i = 0; i < size; ++i)
			ret.c[i] += rhs.c[i];
		return ret;
	}

	Matrix operator+(const value_type rhs) {
		Matrix ret(*this);
		for (int i = 0; i < size; ++i)
			ret.c[i] += rhs;
		return ret;
	}

	Matrix operator*(const Matrix &rhs) {
		if (shape.second != rhs.shape.first)
			throw std::invalid_argument("bad matrix shape");
		Matrix ret({shape.first, rhs.shape.second});
		const auto inners = shape.second;
		const auto &[rows, columns] = ret.shape;
		for (int y = 0; y < rows; y++) {
			for (int i = 0; i < inners; i++) {
				for (int x = 0; x < columns; x++) {
				ret.c[y * columns + x] +=
					c[y * inners + i] * rhs.c[i * columns + x];
		} } }
		return ret;
	}

	bool operator==(const Matrix &rhs) {
		if (size != rhs.size || shape != rhs.shape)
			return false;
		for (int i = 0; i < size; ++i) {
			if (c[i] != rhs.c[i])
				return false;
		}
		return true;
	};

	// TODO: random generator initialization
	// namespace generator {
	// 	value_type zero(value_type) { return 0; }
	// 	value_type norm(value_type n) { return gen(engine) }

	// } // generator
};

