
#include "header.hpp"

template<typename T> /* TODO: compile-time size allocation */
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
	//  for (std::size_t i = 0; i < size; ++i)
	//   m_field[i] = other.m_field[i];
	// };
	// aligned_ptr &operator=(const aligned_ptr &other) {
	//  delete[] m_field;
	//  m_field = new (std::align_val_t{ alignof(T) }) T[other.size];
	//  size = other.size;
	//  for (std::size_t i = 0; i < size; ++i)
	//   m_field[i] = other.m_field[i];
	//  return *this;
	// };

	/* restrict use */
	// aligned_ptr(const aligned_ptr &) = delete;
	// aligned_ptr &operator=(const aligned_ptr &) = delete;

	aligned_ptr(aligned_ptr &&other) noexcept : m_field(other.m_field) { other.m_field = nullptr; };
	aligned_ptr &operator=(aligned_ptr &&other) noexcept { m_field = other.m_field; other.m_field = nullptr; return *this; };
	~aligned_ptr() {};

	__attribute__((always_inline)) T* operator*() { return m_field; };
	__attribute__((always_inline)) std::size_t operator-(aligned_ptr<T> &rhs) { return m_field - rhs.m_field; };
};


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
	
	template <class U>
	linear_alloc(const linear_alloc<U, N>&) noexcept {}
	// pointer allocate(size_type n) { return static_cast<pointer>(malloc(n)); };
	// void deallocate (pointer ptr, size_type n) { };

	__attribute__((always_inline))
	const_size_type align_forward(const aligned_ptr<T> ptr) noexcept {
		const auto tmp_ptr = reinterpret_cast<uintptr_t>(*ptr);
		const auto aligned = (tmp_ptr - 1u + m_alignment) & -m_alignment;
		return aligned - tmp_ptr;
	}

	__attribute__((always_inline))
	aligned_ptr<T> advance(const aligned_ptr<T> p, const_size_type amount) noexcept {
		return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(p.m_field) + amount);
	}

	__attribute__((always_inline))
	aligned_ptr<T> unwind(const aligned_ptr<T> p, const_size_type amount) noexcept {
		return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(p.m_field) - amount);
	}

	pointer allocate(size_type n) noexcept {
		const_size_type adjustment = align_forward(m_current);
		aligned_ptr<T> ptr = advance(m_current, n);
		m_current = advance(ptr, n);
		m_used_bytes = m_current - m_start;
		++m_num_allocations;
		return *ptr;
	}

	void deallocate([[maybe_unused]] pointer, [[maybe_unused]] size_type) {}

private:
	size_type m_alignment{ alignof(T) };
	size_type m_size { N };
	size_type m_used_bytes;
	size_type m_num_allocations;
	aligned_ptr<T> m_start;
	aligned_ptr<T> m_current;
};

using namespace std;

template <int N>
struct X {
	X() noexcept = default;
	int v { N };
};

int main() {
	std::vector<int, linear_alloc<int, 50 * sizeof(int)>> x(30);

	// X<20> x;

	// cout << x.v << endl;


}













/*


template <class T>
struct custom_allocator {
	typedef T value_type;
	custom_allocator() noexcept {}
	template <class U> custom_allocator (const custom_allocator<U>&) noexcept {}
	T* allocate (std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
	void deallocate (T* p, std::size_t n) { ::delete(p); }
};

template <class T, class U>
constexpr bool operator== (const custom_allocator<T>&, const custom_allocator<U>&) noexcept
{return true;}
	
template <class T, class U>
constexpr bool operator!= (const custom_allocator<T>&, const custom_allocator<U>&) noexcept
{return false;}

template <class T>
struct LinearAlloc {
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_t;
	using pointer = T*;

	template <class U>
	LinearAlloc (const LinearAlloc<U>&) noexcept {}

	// LinearAlloc(const LinearAlloc&) = delete;
	// LinearAlloc &operator=(const LinearAlloc&) = delete;
	// LinearAlloc(LinearAlloc &&other) noexcept = default;
	// LinearAlloc &operator=(LinearAlloc &&rhs) noexcept = default;
	LinearAlloc() noexcept = default;
	~LinearAlloc() { delete[] *m_start; };

	[[nodiscard]] pointer allocate(size_type n) noexcept {
		const_size_type adjustment = align_forward(m_current);
		aligned_ptr<T> ptr = advance(m_current, n);
		m_current = advance(ptr, n);
		m_used_bytes = reinterpret_cast<uintptr_t>(m_current) 
						- reinterpret_cast<uintptr_t>(m_start);
		++m_num_allocations;
		return *ptr;
	}

	void deallocate([[maybe_unused]] pointer, [[maybe_unused]] size_type) {}

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


*/