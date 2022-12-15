
// #include <iostream>
// #include <cstddef>
// #include <vector>
// #include <array>
// #include <tuple>
// #include <initializer_list>

// struct slice {
// 	slice();
// 	slice(size_t, size_t, size_t);
// 	explicit slice(size_t = -1);

// 	static slice all;

// 	size_t begin;
// 	size_t length;
// 	size_t offset;
// };

// slice::slice() : begin(-1), length(-1), offset(1) {}

// slice::slice(size_t b, size_t l, size_t o) : begin(b), length(l), offset(o) {}

// explicit slice::slice(size_t b) : begin(b), length(-1), offset(1) {}

// template<size_t N>
// struct MatrixSlice {
// 	MatrixSlice() = default;

// 	MatrixSlice(size_t s, std::initializer_list<size_t> exts);
// 	MatrixSlice(size_t s, std::initializer_list<size_t> exts, std::initializer_list<size_t> offs);

// 	template<typename... Dims>
// 	MatrixSlice(Dims... dims);
// 	template<typename... Dims>
// 	size_t operator()(Dims...) const;

// 	size_t size;
// 	size_t begin;
// 	std::array<size_t, N> extents;
// 	std::array<size_t, N> offsets;
// };

// template<size_t N>
// template<typename... Dims>
// size_t MatrixSlice<N>::operator()(Dims... dims) const {
// 	static_assert(sizeof...(Dims) == N, "");
// 	size_t args[N] { size_t(dims)... };
// 	return inner_product(args, args + N, offsets.begin(), size_t(0));
// }

// template<typename T, size_t N>
// class Matrix {
// public:
// 	static constexpr size_t dims = N;
// 	using value_type = T;
// 	using iter = typename std::vector<T>::iterator;
// 	using const_iter = typename std::vector<T>::const_iterator;

// 	Matrix() = default;
// 	Matrix(Matrix &&) = default;
// 	Matrix &operator=(Matrix &&) = default;
// 	Matrix(Matrix &) = default;
// 	Matrix &operator=(Matrix &) = default;
// 	~Matrix() = default;

// 	template<typename... Exts>
// 	Matrix<T, N>::Matrix(Exts...);

// 	/*
// 	MatrixRef – ссылка на под-матрицу.
// 	*/
// 	template<typename U>
// 	Matrix(const MatrixRef<U, N> &);
// 	template<typename U>
// 	Matrix& operator=(const MatrixRef<U, N> &);

// 	Matrix(MatrixInitializer<T, N>);
// 	Matrix &operator=(MatrixInitializer<T, N>);

// 	/*
// 	Запрет универсальной инициализации.
// 	*/
// 	template<typename U>
// 	Matrix(std::initializer_list<U>) = delete;
// 	template<typename U>
// 	Matrix &operator=(std::initializer_list<U>) = delete;

// private:
// 	MatrixSlice<N> desc;
// 	std::vector<T> elems;
// };

// template<typename T, size_t N>
// template<typename... Exts>
// Matrix<T, N>::Matrix(Exts... exts) : desc(exts...), elems(desc.size) {}
