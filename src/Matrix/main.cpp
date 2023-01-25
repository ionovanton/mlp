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

    static void init(const_size_type n) {
        allocator_state<value_type>::m_start = static_cast<std::shared_ptr<value_type[]>>
        (new (std::align_val_t{ alignof(value_type) }) value_type[n]);
        allocator_state<value_type>::m_current = allocator_state<value_type>::m_start.get();
        allocator_state<value_type>::m_num_allocations = 0;
        allocator_state<value_type>::m_size = n;
    }
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
};

template<class T, class U>
bool operator==(const linear_alloc<T>&, const linear_alloc<U>&) { return true; }
 
template<class T, class U>
bool operator!=(const linear_alloc<T>&, const linear_alloc<U>&) { return false; }


template<typename T, std::size_t N>
struct test {
    test() : a(N / 2), b(N / 2) {};
    std::vector<T, linear_alloc<T>> a;
    std::vector<T, linear_alloc<T>> b;
};

using namespace std;

int main() {
    using program_type = float;
    const size_t n = 6;
    allocator_state<program_type>::init(n);
    test<program_type, n> t;

	t.a[0] = 41;
	t.a[1] = 23;
	t.a[2] = 890;
	t.b[0] = 99;
	t.b[1] = 1;
	t.b[2] = 33;

	inspect_container(t.a);
	inspect_container(t.b);

	auto ptr = &t.a[0];
	cout << *ptr << endl;

	for (int i = 0; i < n; ++i, ++ptr)
		cout << ptr << ' ' << *ptr << '\n';
}
