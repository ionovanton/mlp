#include "header.hpp"
#include "Network.hpp"

using namespace std;

using value_type = int;
using size_type = std::size_t;
using alloc_type = LinearAlloc<value_type>;
template<size_type N, size_type M>
using smatrix = StaticMatrix<value_type, N, M>;
using dmatrix = DynamicMatrix<value_type>;

void test_static()
{
	smatrix<3, 2> a({1, 2, 3, 4, 5, 6});
	smatrix<2, 3> b({1, 2, 3, 4, 5, 6});
	smatrix<2, 4> c({3, 4, 5, 6, 7, 8, 9, 10});
	smatrix<3, 3> check_against_1({9, 12, 15, 19, 26, 33, 29, 40, 51});
	smatrix<2, 4> check_against_2({4, 5, 6, 7, 8, 9, 10, 11});
	smatrix<3, 2> check_against_3({2, 4, 6, 8, 10, 12});

	auto res_1 = a * b;
	auto res_2 = c + 1;
	auto res_3 = a + a;

	cout << (res_1 == check_against_1) << endl;
	cout << (res_2 == check_against_2) << endl;
	cout << (res_3 == check_against_3) << endl;
}

void test_dynamic()
{
	dmatrix a({1, 2, 3, 4, 5, 6}, {3, 2});
	dmatrix b({1, 2, 3, 4, 5, 6}, {2, 3});
	dmatrix c({3, 4, 5, 6, 7, 8, 9, 10}, {2, 4});
	dmatrix check_against_1({9, 12, 15, 19, 26, 33, 29, 40, 51}, {3, 3});
	dmatrix check_against_2({4, 5, 6, 7, 8, 9, 10, 11}, {2, 4});
	dmatrix check_against_3({2, 4, 6, 8, 10, 12}, {3, 2});

	auto res_1 = a * b;
	auto res_2 = c + 1;
	auto res_3 = a + a;

	cout << (res_1 == check_against_1) << endl;
	cout << (res_2 == check_against_2) << endl;
	cout << (res_3 == check_against_3) << endl;
}

int main() {	
	alloc_type::init(2000);
	test_static();
	test_dynamic();
}

/*

TODO:
(!!!) - если перегруженные операторы
используют аллокацию в динамических матрицах,
то это огромная проблема :)
Наверное, придется узнавать кто и сколько раз вызывает
аллокатор.


*/