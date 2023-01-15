
#include "header.hpp"



template <typename T>
struct LinearAlloc {
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_t;
	using pointer = void*;
	using const_pointer = const void*;

	template <class U>
	LinearAlloc (const LinearAlloc<U>&) noexcept {}

	LinearAlloc(const size_type &n) noexcept
	: m_alignment{alignof(value_type)},
	m_size{n},
	m_used_bytes{0},
	m_num_allocations{0},
	m_start{aligned_alloc(m_alignment, n)},
	m_current{m_start} {};

	LinearAlloc(const LinearAlloc&) = delete;
	LinearAlloc &operator=(const LinearAlloc&) = delete;
	LinearAlloc(LinearAlloc &&other) noexcept
	: m_size(other.m_size),
	m_used_bytes(other.m_used_bytes),
	m_num_allocations(other.m_num_allocations),
	m_current(other.m_current),
	m_start(other.m_start) {
		other.m_start = nullptr;
		other.m_current = nullptr;
		other.m_num_allocations = 0;
		other.m_used_bytes = 0;
		other.m_size = 0;
	};
	LinearAlloc &operator=(LinearAlloc &&rhs) noexcept {
		m_size = rhs.m_size;
		m_used_bytes = rhs.m_used_bytes;
		m_num_allocations = rhs.m_num_allocations;
		m_current = rhs.m_current;
		m_start = rhs.m_start;

		rhs.m_start = nullptr;
		rhs.m_current = nullptr;
		rhs.m_num_allocations = 0;
		rhs.m_used_bytes = 0;
		rhs.m_size = 0;

		return *this;
	}

	~LinearAlloc() { free(m_start); };

	[[nodiscard]] pointer allocate(const_size_type n) noexcept {
		const_size_type adjustment = align_forward(m_current);
		pointer aligned_ptr = advance(m_current, n);
		m_current = advance(aligned_ptr, n);
		m_used_bytes = reinterpret_cast<uintptr_t>(m_current) 
						- reinterpret_cast<uintptr_t>(m_start);
		++m_num_allocations;
		return aligned_ptr;
	}

	void deallocate([[maybe_unused]] pointer, [[maybe_unused]] size_type) {}

	__attribute__((always_inline))
	const_size_type align_forward(const_pointer ptr, const_size_type n) noexcept {
		const auto tmp_ptr = reinterpret_cast<uintptr_t>(ptr);
		const auto aligned = (tmp_ptr - 1u + n) & -n;
		return aligned - tmp_ptr;
	}

	__attribute__((always_inline))
	pointer advance(const_pointer p, const_size_type amount) noexcept {
		return reinterpret_cast<pointer>(reinterpret_cast<uintptr_t>(p) + amount);
	}

	__attribute__((always_inline))
	pointer unwind(const_pointer p, const_size_type amount) noexcept {
		return reinterpret_cast<pointer>(reinterpret_cast<uintptr_t>(p) - amount);
	}

	LinearAlloc allocator_type();

private:
	size_type m_alignment;
	size_type m_size;
	size_type m_used_bytes;
	size_type m_num_allocations;
	pointer m_start;
	pointer m_current;
};

template<class T, class U>
bool operator==(const LinearAlloc <T>&, const LinearAlloc <U>&) { return true; }
 
template<class T, class U>
bool operator!=(const LinearAlloc <T>&, const LinearAlloc <U>&) { return false; }

// using namespace std;

int main() {
	constexpr size_t n = 20000000000;
	// LinearAlloc<float> x(n);

	// TODO: fix error
	//		/usr/include/c++/12.2.0/bits/stl_vector.h:551:57: error: no matching function for call to ‘LinearAlloc<int>::LinearAlloc()’
	//		vector(size_type __n, const allocator_type& __a = allocator_type())
	std::vector<int, LinearAlloc<int>> a(20);
	// vector<float> f(20);
}



// template <class T>
// struct custom_allocator {
//   typedef T value_type;
//   custom_allocator() noexcept {}
//   template <class U> custom_allocator (const custom_allocator<U>&) noexcept {}
//   T* allocate (std::size_t n) { return static_cast<T*>(::operator new(n*sizeof(T))); }
//   void deallocate (T* p, std::size_t n) { ::delete(p); }
// };

// template <class T, class U>
// constexpr bool operator== (const custom_allocator<T>&, const custom_allocator<U>&) noexcept
// {return true;}

// template <class T, class U>
// constexpr bool operator!= (const custom_allocator<T>&, const custom_allocator<U>&) noexcept
// {return false;}

// int main () {
//   std::vector<int,custom_allocator<int>> foo = {10,20,30};
//   for (auto x: foo) std::cout << x << " ";
//   std::cout << '\n';
//   return 0;
// }
