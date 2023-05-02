
#include <initializer_list>
#include <functional>

#include "Matrix.hpp"
#include "csv.hpp"

template<typename T>
struct CostFunction {
	using value_type = T;
	using f_two = std::function<value_type(value_type, value_type)>;
	using f_three = std::function<value_type(value_type, value_type, value_type)>;

	CostFunction(f_two f, f_three delta) : f{f}, delta{delta} {}
	virtual ~CostFunction() = default;

	f_two f;
	f_three delta;
};

template<typename T>
struct QuadraticCost : public CostFunction<T> {
	QuadraticCost() : CostFunction<T>(nullptr, nullptr) {};
};

// TODO: Network MUST be abstract
template<typename T>
class Network {
public:
	using value_type = T;
	using size_type = std::size_t;
	using initializer_list = std::initializer_list<size_type>;
	using cost_function = CostFunction<value_type>;

	using dmatrix = DynamicMatrix<value_type>;
	template<size_type N, size_type M>
	using smatrix = StaticMatrix<value_type, N, M>;

	Network(size_type hidden_layer_count, initializer_list layer_sizes, cost_function cf)
	: hidden_layer_count{hidden_layer_count}, layer_sizes{layer_sizes}, cf{cf} {}

protected:
	size_type hidden_layer_count;
	initializer_list layer_sizes;
	cost_function cf;
	dmatrix dataset_train;
	dmatrix dataset_test;
};

template<typename T>
class NetworkMatrix : public virtual Network<T> {
	using value_type = typename Network<T>::value_type;
	using size_type = typename Network<T>::std::size_t;
	using initializer_list = typename Network<T>::std::initializer_list<size_type>;
	using cost_function = typename Network<T>::CostFunction<value_type>;
	using dmatrix = typename Network<T>::dmatrix;
	template<size_type N, size_type M>
	using smatrix = typename Network<T>::smatrix<N, M>;

	using Base = Network<value_type>;
	using dpair = std::pair<dmatrix, dmatrix>;
	template<size_type N, size_type M>
	using spair = std::pair<smatrix<N, M>, smatrix<N, M>>;
	using container = std::vector<dpair>;
	using alloc_type = LinearAlloc<value_type>;

	container layers; // biases, weights

public:
	NetworkMatrix(size_type hidden_layer_count, initializer_list layer_sizes, cost_function cf)
	: Base(hidden_layer_count, layer_sizes, cf), layers{nullptr} {
		alloc_type::init(2000);
	}

	// TODO: parser csv to dmatrix
};
