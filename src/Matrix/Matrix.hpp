
#include <initializer_list>
#include <utility>

template<typename T, typename Alloc>
class Matrix {
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_t;
	using pointer = T*;
	using container = std::vector<value_type, Alloc>;
	using pair = std::pair<size_type, size_type>;

	container mat;
	pair shape;

public:
	Matrix(std::initializer_list init_list, pair pair) : mat{init_list}, shape{pair} {}
	Matrix(pair pair) : mat{pair.first * pair.second}, shape{pair} {}
	Matrix() = delete;
	Matrix(const Matrix&) = delete;
	Matrix &operator=(const Matrix&) = delete;
	Matrix(Matrix&&) = default;
	Matrix &operator=(Matrix&&) = default;
};

