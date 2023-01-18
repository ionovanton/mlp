
#include "header.hpp"

template<typename T>
struct aligned_ptr {
	T *m_field;
	std::size_t size;
	aligned_ptr(const size_t size = 0) : 
	m_field{new (std::align_val_t{ alignof(T) }) T[size]}, size{size} {};

	/* shallow copy */
	aligned_ptr(const aligned_ptr &other) : m_field(other.m_field) {};
	aligned_ptr &operator=(const aligned_ptr &rhs) { m_field = rhs.m_field; };

	/* deep copy */
	// aligned_ptr(const aligned_ptr &other) :
	// m_field{new (std::align_val_t{ alignof(T) }) T[other.size]}, size{other.size} {
	// 	for (std::size_t i = 0; i < size; ++i)
	// 		m_field[i] = other.m_field[i];
	// };
	// aligned_ptr &operator=(const aligned_ptr &other) {
	// 	delete[] m_field;
	// 	m_field = new (std::align_val_t{ alignof(T) }) T[other.size];
	// 	size = other.size;
	// 	for (std::size_t i = 0; i < size; ++i)
	// 		m_field[i] = other.m_field[i];
	// 	return *this;
	// };

	/* restrict use */
	// aligned_ptr(const aligned_ptr &) = delete;
	// aligned_ptr &operator=(const aligned_ptr &) = delete;

	aligned_ptr(aligned_ptr &&other) noexcept : m_field(other.m_field) { other.m_field = nullptr; };
	aligned_ptr &operator=(aligned_ptr &&other) noexcept { m_field = other.m_field; other.m_field = nullptr; };
	~aligned_ptr() {};

	__attribute__((always_inline)) T* operator*() { return m_field; };
};

template <class T>
struct LinearAlloc {
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_t;

	template <class U>
	LinearAlloc (const LinearAlloc<U>&) noexcept {}

	LinearAlloc(const size_type &n) noexcept
	: m_alignment{alignof(value_type)},
	m_size{n},
	m_used_bytes{0},
	m_num_allocations{0},
	m_start{},
	m_current{m_start} {};

	LinearAlloc(const LinearAlloc&) = delete;
	LinearAlloc &operator=(const LinearAlloc&) = delete;
	LinearAlloc(LinearAlloc &&other) noexcept = default;
	LinearAlloc &operator=(LinearAlloc &&rhs) noexcept = default;
	~LinearAlloc() { delete[] *m_start; };

	[[nodiscard]] aligned_ptr<T> allocate(const_size_type n) noexcept {
		const_size_type adjustment = align_forward(m_current);
		aligned_ptr<T> ptr = advance(m_current, n);
		m_current = advance(ptr, n);
		m_used_bytes = reinterpret_cast<uintptr_t>(m_current) 
						- reinterpret_cast<uintptr_t>(m_start);
		++m_num_allocations;
		return ptr;
	}

	void deallocate([[maybe_unused]] aligned_ptr<T>, [[maybe_unused]] size_type) {}

	__attribute__((always_inline))
	const_size_type align_forward(const aligned_ptr<T> ptr, const_size_type n) noexcept {
		const auto tmp_ptr = reinterpret_cast<uintptr_t>(ptr);
		const auto aligned = (tmp_ptr - 1u + n) & -n;
		return aligned - tmp_ptr;
	}

	__attribute__((always_inline))
	aligned_ptr<T> advance(const aligned_ptr<T> p, const_size_type amount) noexcept {
		return reinterpret_cast<aligned_ptr<T>>(reinterpret_cast<uintptr_t>(p) + amount);
	}

	__attribute__((always_inline))
	aligned_ptr<T> unwind(const aligned_ptr<T> p, const_size_type amount) noexcept {
		return reinterpret_cast<aligned_ptr<T>>(reinterpret_cast<uintptr_t>(p) - amount);
	}


private:
	size_type m_alignment;
	size_type m_size;
	size_type m_used_bytes;
	size_type m_num_allocations;
	aligned_ptr<T> m_start;
	aligned_ptr<T> m_current;
};

template<class T, class U>
bool operator==(const LinearAlloc <T>&, const LinearAlloc <U>&) { return true; }
 
template<class T, class U>
bool operator!=(const LinearAlloc <T>&, const LinearAlloc <U>&) { return false; }

using namespace std;





int main() {
	vector<float, LinearAlloc<float>> a(20);
	// TODO: fix error
			// /usr/include/c++/12.2.0/bits/stl_vector.h:551:57: error: no matching function for call to ‘LinearAlloc<int>::LinearAlloc()’
			// vector(size_type __n, const allocator_type& __a = allocator_type())
	// vector<float> f(20);
}



// template <class T>
// struct custom_allocator {
// 	typedef T value_type;
// 	custom_allocator() noexcept {}
// 	template <class U> custom_allocator (const custom_allocator<U>&) noexcept {}
// 	T* allocate (std::size_t n) { return static_cast<T*>(::operator new(n*sizeof(T))); }
// 	void deallocate (T* p, std::size_t n) { ::delete(p); }
// };

// template <class T, class U>
// constexpr bool operator== (const custom_allocator<T>&, const custom_allocator<U>&) noexcept
// {return true;}

// template <class T, class U>
// constexpr bool operator!= (const custom_allocator<T>&, const custom_allocator<U>&) noexcept
// {return false;}

// int main () {
// 	std::vector<int,custom_allocator<int>> foo = {10,20,30};
// 	for (auto x: foo) std::cout << x << " ";
// 	std::cout << '\n';
// 	return 0;
// }
