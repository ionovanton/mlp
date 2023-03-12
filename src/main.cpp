#include "header.hpp"
#include "LinearAlloc.hpp"
// #include "NetworkMatrixImpl.hpp"
#include "Matrix.hpp"

using namespace std;


using value_type = int;
using size_type = std::size_t;
using alloc_type = LinearAlloc<value_type>;
using dmatrix = DynamicMatrix<value_type>;
template<size_type N>
using smatrix = StaticMatrix<value_type, N>;


int main() {

	// template<typename >
	
	alloc_type::init(2000);

	{
		dmatrix a({1, 2, 3, 4, 5, 6}, {3, 2});
		dmatrix b({1, 2, 3, 4, 5, 6}, {2, 3});
		dmatrix c({3, 4, 5, 6, 7, 8, 9, 10}, {2, 4});
		dmatrix check_against_1({9, 12, 15, 19, 26, 33, 29, 40, 51}, {3, 3});
		dmatrix check_against_2({4, 5, 6, 7, 8, 9, 10, 11}, {2, 4});

		auto res_1 = a * b;
		auto res_2 = c + 1;

		cout << (res_1 == check_against_1) << endl;
		cout << (res_2 == check_against_2) << endl;
	}

	{
		smatrix<6> a({1, 2, 3, 4, 5, 6}, {3, 2});
		smatrix<6> b({1, 2, 3, 4, 5, 6}, {2, 3});
		smatrix<8> c({3, 4, 5, 6, 7, 8, 9, 10}, {2, 4});
		smatrix<9> check_against_1({9, 12, 15, 19, 26, 33, 29, 40, 51}, {3, 3});
		smatrix<8> check_against_2({4, 5, 6, 7, 8, 9, 10, 11}, {2, 4});

		auto res_1 = a * b;
		auto res_2 = c + 1;

		// cout << (res_1 == check_against_1) << endl;
		// cout << (res_2 == check_against_2) << endl;
	}

	// NetworkMatrixImpl<2, value_type, alloc_type> net({2, 3});



	// SomeContainer<int, 5> x({1, 2, 3, 7, 8});
	// SomeContainer<int, 3> y({20, 21, 22});
	// SomeContainer<int, 8> z = x + y;

	// z.inspect();

	// std::array<int, 3> test({1,42,3});

	// for (auto e : test)
	// 	cout << e << endl;

}

/*

TODO:
(!!!) - если перегруженные операторы
используют аллокацию в динамических матрицах,
то это огромная проблема :)
Наверное, придется узнавать кто и сколько раз вызывает
аллокатор.


remove all exceptions
Make two variants:
	matrix with std::array -> template should be able to be instantiated with Container argument
	all tmp matricies will use heap linear allocator:
		one allocator
		multiple allocator with different states

*/
