#include "header.hpp"

template<typename C>
void inspect_container(C &c) {
	std::cout << "vector: " << &c << "\n";
	std::cout << "	size: " << c.size() << "\n";
	std::cout << "	cap: " << c.capacity() << "\n";
	for (auto &e : c) {
		std::cout << "	" << &e << " " << e << "\n";
	}
	std::cout << std::endl;
}

template<typename>
class linear_alloc;

template<typename T>
class allocator_state {
	friend class linear_alloc<T>;
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_t;
	using pointer = T*;

	static std::shared_ptr<value_type[]> m_start;
	static pointer m_current;
	static size_type m_num_allocations;
	static size_type m_size;

public:
    allocator_state() = delete;
    allocator_state(const allocator_state&) = delete;
    allocator_state &operator=(const allocator_state&) = delete;
    allocator_state(const allocator_state&&) = delete;
    allocator_state &operator=(const allocator_state&&) = delete;
};

template<typename T>
std::shared_ptr<T[]> allocator_state<T>::m_start;

template<typename T>
T* allocator_state<T>::m_current;

template<typename T>
std::size_t allocator_state<T>::m_num_allocations;

template<typename T>
std::size_t allocator_state<T>::m_size;

template<typename T>
struct linear_alloc {
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_t;
	using pointer = T*;

	linear_alloc(const linear_alloc&) noexcept = default;
	linear_alloc() noexcept = default;
	~linear_alloc() = default;
	
	/* what is this for? */
	template <class U>
	linear_alloc(const linear_alloc<U>&) noexcept {}

	[[nodiscard]] pointer allocate(size_type n) noexcept {
		auto ret_ptr = allocator_state<T>::m_current;
		allocator_state<T>::m_current += n;
		allocator_state<T>::m_num_allocations += n;
		return ret_ptr;
	}

	void deallocate([[maybe_unused]] pointer, [[maybe_unused]] size_type) {}

    static void init(const_size_type n) {
        allocator_state<value_type>::m_start = static_cast<std::shared_ptr<value_type[]>>
        (new (std::align_val_t{ alignof(value_type) }) value_type[n]);
        allocator_state<value_type>::m_current = allocator_state<value_type>::m_start.get();
        allocator_state<value_type>::m_num_allocations = 0;
        allocator_state<value_type>::m_size = n;
    }
};

template<class T, class U>
bool operator==(const linear_alloc<T>&, const linear_alloc<T>&) { return true; }
 
template<class T, class U>
bool operator!=(const linear_alloc<T>&, const linear_alloc<U>&) { return false; }


/**
 * @param pair container which values are @arg @c y-count and @arg @c column-count
 * @param size total elements
 * @param c container
 */
template<typename T, typename Alloc>
class Matrix {
private:
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_type;
	using container = std::vector<value_type, Alloc>;
	using pair = std::pair<size_type, size_type>;

	pair shape;
	size_type size;
	container c;

	void inspect() {
		std::cout << "Matrix: " << this << "\n" << "Shape: " << shape.first << 'x' << shape.second  << "\n";
		std::cout << "{";
		auto &[rows, columns] = shape;
		for (int y = 0; y < rows; ++y) {
			std::cout << (y == 0 ? " " : "  ");
			for (int x = 0; x < columns; ++x) {
				std::cout << c[x + y * columns] << ' ';
			}
			std::cout << (y == rows - 1 ? "" : "\n");
		}
		std::cout << " }\n\n";
	}

public:
	Matrix() = delete;
	Matrix(std::initializer_list<value_type> li, pair shape) : shape{shape}, size{shape.first * shape.second}, c{li} {}
	Matrix(pair shape) : shape{shape}, size{shape.first * shape.second}, c(size) {}

	Matrix(const Matrix&) = default;
	Matrix &operator=(const Matrix&) = default;
	Matrix(Matrix&&) noexcept = default;
	Matrix &operator=(Matrix&&) noexcept = default;

	~Matrix() { inspect(); };

	Matrix operator+(const Matrix &rhs) {
		if (shape != rhs.shape)
			throw std::invalid_argument("bad_shape");
		Matrix ret(shape);
		for (int i = 0; i < size; ++i)
			ret.c[i] = c[i] + rhs.c[i];
		return ret;
	}

	Matrix operator*(const Matrix &rhs) {
		if (shape.second != rhs.shape.first)
			throw std::invalid_argument("bad_shape");
		Matrix ret({shape.first, rhs.shape.second});
		const auto inners = shape.second;
		const auto &[rows, columns] = ret.shape;
		for (int y = 0; y < rows; y++) {
			for (int i = 0; i < inners; i++) {
				for (int x = 0; x < columns; x++) {
				ret.c[y * columns + x] +=
					c[y * inners + i] * rhs.c[i * columns + x];
		} } }
		return ret;
	}

	bool operator==(const Matrix &rhs) {
		return size == rhs.size && shape == rhs.shape && c == rhs.c;
	};
};

using namespace std;

int main() {
	using value_type = int;
	using alloc_type = linear_alloc<value_type>;
	using matrix = Matrix<value_type, alloc_type>;

	alloc_type::init(100);

	matrix a({1,2,3,4,5,6}, {3, 2});
	matrix b({1,2,3,4,5,6}, {2, 3});


	auto c = a * b;
	// auto d = a + b;



}
