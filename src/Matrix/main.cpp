#include "Matrix.hpp"

int main() {
	auto t = std::make_tuple(3, 5);
	Matrix<double, 2> a;

	std::cout << std::get<0>(t) << std::endl;
}
