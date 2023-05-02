
#include <initializer_list>
#include <algorithm>
#include <utility>
#include <type_traits>
#include <vector>

#include "LinearAlloc.hpp"

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

template<typename T, std::size_t N, std::size_t M>
class StaticMatrix;

template<typename T, typename C>
class Matrix {
	template<typename U, std::size_t N, std::size_t M>
	friend class StaticMatrix;

	using value_type = T;
	using container = C;
	using size_type = std::size_t;
	using pair = std::pair<size_type, size_type>;

	pair shape;
	size_type size;
	container c;

public:
	Matrix() : size{nullptr}, shape{nullptr}, c{nullptr} {}
	Matrix(pair shape, int) : size{shape.first * shape.second}, shape{shape}, c{} {}
	Matrix(pair shape) : size{shape.first * shape.second}, shape{shape}, c(shape.first * shape.second) {}
	Matrix(container c, pair shape) : size{shape.first * shape.second}, shape{shape}, c{c} {}
	virtual ~Matrix() {
		// this->inspect();
	}

	bool operator==(const Matrix &rhs) {
		if (size != rhs.size || shape != rhs.shape)
			return false;
		for (int i = 0; i < size; ++i) {
			if (c[i] != rhs.c[i])
				return false;
		}
		return true;
	}

	Matrix operator+(const value_type rhs) {
		Matrix ret(*this);
		for (int i = 0; i < size; ++i)
			ret.c[i] += rhs;
		return ret;
	}

	Matrix operator+(const Matrix &rhs) {
		if (shape != rhs.shape)
			throw std::invalid_argument("bad matrix shape");
		Matrix ret(*this);
		for (int i = 0; i < size; ++i)
			ret.c[i] += rhs.c[i];
		return ret;
	}

	Matrix operator*(const Matrix &rhs) {
		if (shape.second != rhs.shape.first)
			throw std::invalid_argument("bad matrix shape");
		Matrix ret = Matrix({shape.first, rhs.shape.second});
		const auto inners = rhs.shape.first;
		const auto &[rows, columns] = ret.shape;
		for (int y = 0; y < rows; y++) {
			for (int i = 0; i < inners; i++) {
				for (int x = 0; x < columns; x++) {
				ret.c[y * columns + x] +=
					c[y * inners + i] * rhs.c[i * columns + x];
		} } }
		return ret;
	}

protected:
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
};

template<typename T>
class DynamicMatrix : public Matrix<T, std::vector<T, LinearAlloc<T>>> {
	using value_type = T;
	using size_type = std::size_t;
	using alloc_type = LinearAlloc<value_type>;
	using pair = std::pair<size_type, size_type>;
	using container = std::vector<value_type, alloc_type>;
	using Base = Matrix<value_type, container>;

public:
	DynamicMatrix() : Base() {}
	DynamicMatrix(pair &&shape) : Base(shape) {}
	DynamicMatrix(container &&c, pair &&shape) : Base(c, shape) {}
	~DynamicMatrix() = default;
};

template<typename T, std::size_t N, std::size_t M>
class StaticMatrix : public Matrix<T, std::array<T, N * M>> {
	using value_type = T;
	using size_type = std::size_t;
	using pair = std::pair<size_type, size_type>;
	template<typename U, size_type X, size_type Y>
	using container = std::array<U, X * Y>;
	using Base = Matrix<value_type, container<T, N, M>>;

public:
	StaticMatrix() : Base({N, M}, int()) {}
	StaticMatrix(container<T, N, M> &&c) : Base(c, {N, M}) {}
	~StaticMatrix() = default;

	template<std::size_t X, std::size_t Y>
	StaticMatrix<T, N, Y> operator*(const StaticMatrix<T, X, Y> &rhs) {
		auto ret = StaticMatrix<T, N, Y>();
		const auto inners = X;
		const auto rows = N;
		const auto columns = Y;
		for (int y = 0; y < rows; y++) {
			for (int i = 0; i < inners; i++) {
				for (int x = 0; x < columns; x++) {
				ret.c[y * columns + x] +=
					this->c[y * inners + i] * rhs.c[i * columns + x];
		} } }
		return ret;
	}
};
