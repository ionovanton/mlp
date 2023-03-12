
#include <initializer_list>
#include <algorithm>
#include <utility>

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

template<typename T, typename C>
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
	using container = C;
	using size_type = size_t;
	using const_size_type = const size_type;
	using dynamic_container = std::vector<value_type, LinearAlloc<value_type>>;
	using initializer_list = std::initializer_list<value_type>;
	using pair = std::pair<size_type, size_type>;

	pair shape;
	size_type size;
	container c;

public:
	Matrix() : shape({0, 0}), size{0}, c{} {};
	Matrix(initializer_list li, pair &&shape) : shape{shape}, size{shape.first * shape.second}, c{li} {}
	Matrix(pair &&shape) : shape{shape}, size{shape.first * shape.second}, c(size) {}
	Matrix(container &&c, pair &&shape) : shape{shape}, size{c.size()}, c{c} {}

	Matrix(const Matrix&) = default;
	Matrix &operator=(const Matrix&) = default;
	Matrix(Matrix&&) noexcept = default;
	Matrix &operator=(Matrix&&) noexcept = default;

	virtual ~Matrix() {};

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
		Matrix ret;
		if constexpr (std::is_same<container, dynamic_container>::value)
			ret = Matrix(pair{shape.first, rhs.shape.second});
		else
			ret = Matrix(container(), pair{shape.first, rhs.shape.second});
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
};

template<typename T>
class DynamicMatrix : public Matrix<T, std::vector<T, LinearAlloc<T>>> {
	using Base = Matrix<T, std::vector<T, LinearAlloc<T>>, DynamicMatrix<T>>;
	using value_type = T;
	using size_type = std::size_t;
	using pair = std::pair<size_type, size_type>;
	using initializer_list = std::initializer_list<value_type>;
	using container = std::vector<value_type, LinearAlloc<value_type>>;

public:
	DynamicMatrix() : Base() {}
	DynamicMatrix(initializer_list li, pair &&shape) : Base(li, std::move(shape)) {}
	DynamicMatrix(pair &&shape) : Base(std::move(shape)) {}
};

template<typename T, std::size_t N>
class StaticMatrix : public Matrix<T, std::array<T, N>, StaticMatrix<T, N>> {
	using Base = Matrix<T, std::array<T, N>, StaticMatrix<T, N>>;
	using value_type = T;
	using size_type = std::size_t;
	using container = std::array<value_type, N>;
	using pair = std::pair<size_type, size_type>;
	
public:
	StaticMatrix(container &&c, pair &&shape) : Base(std::move(c), std::move(shape)) {}
	StaticMatrix(pair &&shape) : Base::shape(shape) {}
};

template<typename T, std::size_t N>
class SomeContainer {
	template<typename A, std::size_t X, std::size_t Y>
	friend SomeContainer<A, X + Y> operator+(SomeContainer<A, X> &, SomeContainer<A, Y> &);

	std::array<T, N> c;
public:

	void inspect() {
		for (auto e : c) {
			std::cout << e << ' ';
		}
		std::cout << '\n';
	}

	SomeContainer(std::array<T, N> &&x) : c{x} {}
	SomeContainer() = default;
};

template<typename A, std::size_t X, std::size_t Y>
SomeContainer<A, X + Y> operator+(SomeContainer<A, X> &x, SomeContainer<A, Y> &y) {
	SomeContainer<A, X + Y> ret;
	std::copy(x.c.begin(), x.c.end(), ret.c.begin());
	std::copy(y.c.begin(), y.c.end(), ret.c.begin() + X);
	return ret;
}

// int main() {
// 	SomeContainer<int, 5> x({1, 2, 3, 7, 8});
// 	SomeContainer<int, 3> y({20, 21, 22});
// 	SomeContainer<int, 8> z = x + y;

// 	z.inspect();
// 	// 1 2 3 7 8 20 21 22
// }



