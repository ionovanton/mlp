#include <memory>
#include <iostream>

template<typename>
class LinearAlloc;

template<typename T>
class allocator_state {
	friend class LinearAlloc<T>;
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_t;
	using pointer = T*;

	static std::shared_ptr<value_type[]> m_start;
	static pointer m_current;
	static size_type m_num_allocations;
	static size_type m_num_invokations; // debug
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
std::size_t allocator_state<T>::m_num_invokations;

template<typename T>
std::size_t allocator_state<T>::m_size;


template<typename T>
struct LinearAlloc {
	using value_type = T;
	using size_type = size_t;
	using const_size_type = const size_t;
	using pointer = T*;
	using state = allocator_state<value_type>;

	LinearAlloc(const LinearAlloc&) noexcept = default;
	LinearAlloc() noexcept = default;
	~LinearAlloc() = default;

	/* what is this for? */
	template <class U>
	LinearAlloc(const LinearAlloc<U>&) noexcept {}

	[[nodiscard]] pointer allocate(size_type n) noexcept {
		auto ret_ptr = state::m_current;
		state::m_current += n;
		state::m_num_allocations += n;
		++state::m_num_invokations;
		return ret_ptr;
	}

	void deallocate([[maybe_unused]] pointer, [[maybe_unused]] size_type) {}

    static void init(const_size_type n) {
        state::m_start = static_cast<std::shared_ptr<value_type[]>>
        (new (std::align_val_t{ alignof(value_type) }) value_type[n]);
        state::m_current = state::m_start.get();
        state::m_num_allocations = 0;
		state::m_num_invokations = 0;
        state::m_size = n;
    }

	static void inspect_invokation() {
		std::cout << "\nAllocator invokations: " << state::m_num_invokations << "\n";
	}
};

template<class T, class U>
bool operator==(const LinearAlloc<T>&, const LinearAlloc<T>&) { return true; }
 
template<class T, class U>
bool operator!=(const LinearAlloc<T>&, const LinearAlloc<U>&) { return false; }