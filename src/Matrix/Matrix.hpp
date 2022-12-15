
#include <iostream>
#include <cstddef>
#include <vector>
#include <tuple>

template<typename T, size_t N>
class Matrix {
public:
	// static constexpr size_t dim = N;
	using value_type = T;
	using iter = typename std::vector<T>::iterator;
	using const_iter = typename std::vector<T>::const_iterator;

public:
	Matrix() = default;
	Matrix(Matrix &&) = default;
	Matrix &operator=(Matrix &&) = default;
	Matrix(Matrix &) = default;
	Matrix &operator=(Matrix &) = default;
	~Matrix() = default;




};

