
#include "header.hpp"

template <typename T>
struct LinearAlloc {
	using value_type = T;
	using size_type = size_t;
	using pointer = void*;
	using const_pointer = const void*;

	LinearAlloc(const size_type n) 
	: m_size{n},
	m_used_bytes{0},
	m_num_allocations{0},
	m_start{aligned_alloc(alignof(T), n)}
	{ };

	LinearAlloc(const LinearAlloc&) = delete;
	LinearAlloc &operator=(const LinearAlloc&) = delete;
	LinearAlloc(LinearAlloc &&other)
	: m_size(other.m_size),
	m_used_bytes(other.m_used_bytes),
	m_num_allocations(other.m_num_allocations),
	m_current(other.m_current),
	m_start(other.m_start)
	{
		other.m_start = nullptr;
		other.m_current = nullptr;
		other.m_num_allocations = 0;
		other.m_used_bytes = 0;
		other.m_size = 0;
	};
	LinearAlloc &LinearAlloc::operator=(LinearAlloc &&rhs) noexcept
	{
		m_size = rhs.m_size;
		m_usedBytes = rhs.m_usedBytes;
		m_numAllocations = rhs.m_numAllocations;
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

private:
	size_type m_size;
	size_type m_used_bytes;
	size_type m_num_allocations;
	pointer m_current;
	pointer m_start;
};

using namespace std;

int main() {
	const uint_fast64_t n = 20000000000;
	LinearAlloc<float> x(n);

	vector<int> a(20);
	vector<float> f(20);


}
