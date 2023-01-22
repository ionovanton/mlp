
#include "header.hpp"

template<typename T>
struct aligned_ptr {
	using pointer = T*;
	using size_type = std::size_t;
	using const_size_type = const std::size_t;

	pointer m_field;
	size_type size;
	aligned_ptr(const_size_type size = 0) : 
	m_field{new (std::align_val_t{ alignof(T) }) T[size]}, size{size} { class_func_log; }	// aligned allocation
	// m_field{new T[size]}, size{size} { class_func_log; }									// standard allocation

	/* deep copy */
	aligned_ptr(const aligned_ptr<T> &other) :
	m_field{new (std::align_val_t{ alignof(T) }) T[other.size]}, size{other.size} {
		class_func_log;
		for (std::size_t i = 0; i < size; ++i)
			m_field[i] = other.m_field[i];
	};
	aligned_ptr<T> &operator=(const aligned_ptr<T> &other) {
		class_func_log;

		delete[] m_field;
		m_field = new (std::align_val_t{ alignof(T) }) T[other.size];
		size = other.size;
		for (std::size_t i = 0; i < size; ++i)
			m_field[i] = other.m_field[i];
		return *this;
	};

	/* shallow copy */
	// aligned_ptr(const aligned_ptr &other) : m_field{other.m_field}, size{other.size} {};
	// aligned_ptr &operator=(const aligned_ptr &rhs) { m_field = rhs.m_field; size = rhs.size; };
	
	/* no copy */
	// aligned_ptr(const aligned_ptr &) = delete;
	// aligned_ptr &operator=(const aligned_ptr &) = delete;

	aligned_ptr(aligned_ptr &&other) noexcept : m_field{other.m_field} {
		class_func_log;
		other.m_field = nullptr;
	}
	aligned_ptr &operator=(aligned_ptr &&other) noexcept {
		class_func_log;
		m_field = other.m_field;
		other.m_field = nullptr;
		return *this;
	}
	~aligned_ptr() { delete[] m_field; class_func_log; }

	__attribute__((always_inline))
	void advance(size_type n) { m_field += n; }
	__attribute__((always_inline))
	void unwind(size_type n) { m_field -= n; }
	// __attribute__((always_inline))
	// pointer operator*() { return m_field; }
};

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

template<typename T>
void inspect_ptr(aligned_ptr<T> &p) {
	std::cout << "aligned_ptr: " << &p << "\n";
	for (auto i = p.size * 0; i < p.size; ++i)
		std::cout << "	" << &p.m_field[i] << " " << p.m_field[i] << "\n";
	std::cout << std::endl;
}


template <class T, std::size_t N>
struct linear_alloc {
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_t;
	using pointer = T*;

	template<typename U>
	struct rebind {
		using other = linear_alloc<U, N>;
	};

	linear_alloc() noexcept = default;
	~linear_alloc() = default;
	
	/* what is this for? */
	template <class U>
	linear_alloc(const linear_alloc<U, N>&) noexcept {}

	// __attribute__((always_inline))
	// const_size_type  find_align_forward(const aligned_ptr<T> &ptr) noexcept {
	// 	const auto tmp_ptr = reinterpret_cast<uintptr_t>(ptr.m_field);
	// 	const int_fast64_t aligned = (tmp_ptr - 1u + m_alignment) & -m_alignment;
	// 	return aligned - tmp_ptr;
	// }

	// __attribute__((always_inline))
	// pointer advance(const aligned_ptr<T> &ptr, const_size_type amount) noexcept {
	// 	return ptr.m_field + amount;
	// }

	// __attribute__((always_inline))
	// pointer unwind(const aligned_ptr<T> &ptr, const_size_type amount) noexcept {
	// 	return ptr.m_field - amount;
	// }

	pointer allocate(size_type n) noexcept {
		class_func_log;
		auto ret_ptr = m_current;
		m_current += n;
		m_num_allocations += n % m_size;
		return ret_ptr;
	}

	void deallocate([[maybe_unused]] pointer, [[maybe_unused]] size_type) {}

private:
	const aligned_ptr<T> m_start { N };
	pointer m_current { m_start.m_field };
	size_type m_num_allocations { 0 };
	size_type m_size { N };
	// size_type m_used_bytes { 0 };
};

using namespace std;

int main() {
	const int n = 20000;
	using value_type = int;
	using alloc = linear_alloc<value_type, n>;

	std::vector<value_type, alloc> a(n);
	// a[0] = 21;
}
