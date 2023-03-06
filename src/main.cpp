#include "header.hpp"
#include "LinearAlloc.hpp"
#include "NetworkMatrixImpl.hpp"

using namespace std;

int main() {
	using value_type = int;
	using alloc_type = LinearAlloc<value_type>;
	using matrix = Matrix<value_type, alloc_type>;

	// matrix a({1, 2, 3, 4, 5, 6}, {3, 2});
	// matrix b({1, 2, 3, 4, 5, 6}, {2, 3});
	// matrix c({3, 4, 5, 6, 7, 8, 9, 10}, {2, 4});
	// matrix check_against_1({9, 12, 15, 19, 26, 33, 29, 40, 51}, {3, 3});
	// matrix check_against_2({4, 5, 6, 7, 8, 9, 10, 11}, {2, 4});

	// auto res_1 = a * b;
	// auto res_2 = c + 1;

	// cout << (res_1 == check_against_1) << endl;
	// cout << (res_2 == check_against_2) << endl;

	NetworkMatrixImpl<2, value_type, alloc_type> net({2, 3});
}

/*

TODO:
remove all exceptions
Make two variants:
	matrix with std::array -> template should be able to be instantiated with Container argument
	all tmp matricies will use heap linear allocator:
		one allocator
		multiple allocator with different states

*/
