
#include "Matrix.hpp"
#include <initializer_list>
#include <functional>

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

	Network(size_type hidden_layer_count, initializer_list layer_sizes, cost_function cf)
	: hidden_layer_count{hidden_layer_count}, layer_sizes{layer_sizes}, cf{cf} {}

protected:
	size_type hidden_layer_count;
	initializer_list layer_sizes;
	cost_function cf;
};

template<typename T>
class NetworkMatrix : public Network<T> {
	using Base = Network<value_type>;
	using dmatrix = DynamicMatrix<value_type>;
	template<size_type N, size_type M>
	using smatrix = StaticMatrix<value_type, N, M>;
	using dpair = std::pair<dmatrix, dmatrix>;
	template<size_type N, size_type M>
	using dpair = std::pair<smatrix<N, M>, smatrix<N, M>>;

	std::vector<dpair> layers; // biases, weights

public:
	NetworkMatrix(size_type hidden_layer_count, initializer_list layer_sizes, cost_function cf)
	: Base(hidden_layer_count, layer_sizes, cf), layers{} {}
};
