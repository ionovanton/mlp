
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

	linear_alloc(const linear_alloc&) noexcept = default;
	linear_alloc() noexcept = default;
	~linear_alloc() = default;
	
	/* what is this for? */
	template <class U>
	linear_alloc(const linear_alloc<U, N>&) noexcept {}

	[[nodiscard]] pointer allocate(size_type n) noexcept {
		auto ret_ptr = m_current;
		m_current += n;
		m_num_allocations += n;
		return ret_ptr;
	}

	void deallocate([[maybe_unused]] pointer, [[maybe_unused]] size_type) {}

private:
	std::shared_ptr<T[]> m_start{ new (std::align_val_t{ alignof(T) }) T[N] };
	pointer m_current { m_start.get() };
	size_type m_num_allocations { 0 };
	size_type m_size { N };
};

using namespace std;

template<typename T, std::size_t N, typename Alloc>
struct test {

	std::size_t n { N };
	std::vector<T, linear_alloc<T, N>> a { std::vector<T, linear_alloc<T, N>>(N) };
	std::vector<T, linear_alloc<T, N>> b { std::vector<T, linear_alloc<T, N>>(N) };
};

int main() {
	const int n = 2;
	// linear_alloc<int, n> x;

	// std::vector<int, linear_alloc<int, n>> a(n);
	// a[0] = 21;
	// a[1] = 21;
	// a[2] = 21;

	test<int, 10, linear_alloc<int, 10>> x;


}
