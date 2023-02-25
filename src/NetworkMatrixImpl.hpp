
#include "Matrix.hpp"

template<std::size_t LayerCount, typename T, typename Alloc>
class NetworkMatrixImpl {

	// template<std::size_t N, typename K, typename A>
	// struct WeightLayerDense {
	// 	using matrix = Matrix<K, A>;
	// 	using matrix_dense = std::array<matrix, N>;

	// 	matrix input_layer;
	// 	matrix_dense hidden_layer;
	// 	matrix output_layer;
	// };

	// template<std::size_t N, typename K, typename A>
	// struct BiasLayerDense {
	// 	using matrix = Matrix<K, A>;
	// 	using matrix_dense = std::array<matrix, N>;
	// };

	using value_type = T;
	using alloc_type = Alloc;
	using size_type = size_t;
	using const_size_type = const size_type;
	using initializer_list = std::initializer_list<size_type>;
	using matrix = Matrix<value_type, alloc_type>;
	using weight_dense = std::array<matrix, LayerCount+3>;

	size_type element_count;
	weight_dense w;

public:
	NetworkMatrixImpl(initializer_list &&li) : element_count{0} {
		std::vector<size_type> shapes{1, 784, 26, 1};
		shapes.insert(shapes.begin() + 2, li);
		const int n = shapes.size();
		for (int i = 0; i < n - 1; ++i)
			element_count += shapes[i] * shapes[i + 1];
		alloc_type::init(element_count);
		for (int i = 0; i < n - 1; ++i)
			w[i] = matrix({shapes[i], shapes[i + 1]});
	}
	~NetworkMatrixImpl() {

		// debug
		{
			std::cout << "NetworkMatrixImpl info:\n";
			std::cout << element_count << "\n";
			alloc_type::inspect_invokation();
			for (int i = 0; i < LayerCount + 3; ++i)
				w[i].inspect_shape();
			std::cout << std::endl;
		}
	}
};
