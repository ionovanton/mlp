#include "linalloc.hpp"

class Allocator
{
public:
	Allocator(const std::size_t sizeBytes, void* const start) noexcept;

	Allocator(const Allocator&) = delete;
	Allocator& operator=(Allocator&) = delete;
	Allocator(Allocator&&) noexcept;
	Allocator& operator=(Allocator&&) noexcept;

	virtual ~Allocator() noexcept;

	virtual void* Allocate(const std::size_t& size,
		const std::uintptr_t& alignment = sizeof(std::intptr_t)) = 0;

	virtual void Free(void* const ptr) = 0;

	const std::size_t& GetSize() const noexcept;
	const std::size_t& GetUsed() const noexcept;
	const std::size_t& GetNumAllocation() const noexcept;

	const void* GetStart() const noexcept;

protected:

	std::size_t m_size;
	std::size_t m_usedBytes;
	std::size_t m_numAllocations;

	void* m_start;
};

Allocator::Allocator(const std::size_t sizeBytes, void* const start) noexcept
:
    m_size(sizeBytes),
    m_usedBytes(0),
    m_numAllocations(0),
    m_start(start)
{
    assert(sizeBytes > 0);
}

Allocator::Allocator(Allocator&& other) noexcept
:
    m_size(other.m_size),
    m_usedBytes(other.m_usedBytes),
    m_numAllocations(other.m_numAllocations),
    m_start(other.m_start)
{
    other.m_start = nullptr;
    other.m_size = 0;
    other.m_numAllocations = 0;
    other.m_usedBytes = 0;
}

Allocator& Allocator::operator=(Allocator&& rhs) noexcept
{
    m_size = rhs.m_size;
    m_usedBytes = rhs.m_usedBytes;
    m_numAllocations = rhs.m_numAllocations;
    m_start = rhs.m_start;

    rhs.m_start = nullptr;
    rhs.m_size = 0;
    rhs.m_numAllocations = 0;
    rhs.m_usedBytes = 0;

    return *this;
}

Allocator::~Allocator() noexcept
{
    assert(m_numAllocations == 0 && m_usedBytes == 0);
}

const std::size_t& Allocator::GetSize() const noexcept
{
    return m_size;
}

const std::size_t& Allocator::GetUsed() const noexcept
{
    return m_usedBytes;
}

const std::size_t& Allocator::GetNumAllocation() const noexcept
{
    return m_numAllocations;
}

const void* Allocator::GetStart() const noexcept
{
    return m_start;
}






class LinearAllocator : public Allocator
{
public:
	~LinearAllocator() noexcept;
	LinearAllocator(const std::size_t sizeBytes,
					void* const start) noexcept;

	LinearAllocator(const LinearAllocator&) = delete;
	LinearAllocator& operator=(const LinearAllocator&)
		= delete;
	LinearAllocator(LinearAllocator&&) noexcept;
	LinearAllocator& operator=(LinearAllocator&&) noexcept;

	virtual void* Allocate(const std::size_t& size,
						   const std::uintptr_t& alignment
						       = sizeof(std::intptr_t)) override;

	virtual void Free(void* const ptr) noexcept override final;

	void* GetCurrent() const noexcept;

	virtual void Rewind(void* const mark) noexcept;
	virtual void Clear() noexcept;

protected:
	void* m_current;
};
 
template<class T, class U>
bool operator==(const Mallocator <T>&, const Mallocator <U>&) { return true; }
 
template<class T, class U>
bool operator!=(const Mallocator <T>&, const Mallocator <U>&) { return false; }

inline std::size_t align_forward_adjustment
(const void* const ptr, const std::size_t & alignment) noexcept
{
	const auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
	const auto aligned = (iptr - 1u + alignment) & -alignment;
	return aligned - iptr;
}

inline void* ptr_add(const void* const p, const std::uintptr_t& amount) noexcept
{
	return reinterpret_cast<void*>
		(reinterpret_cast<std::uintptr_t>(p) + amount);
}

LinearAllocator::LinearAllocator(const std::size_t sizeBytes,
                                 void* const start) noexcept
:
    Allocator(sizeBytes, start),
    m_current(const_cast<void*>(start))
{

}

LinearAllocator::LinearAllocator(LinearAllocator&& other) noexcept
:
    Allocator(std::move(other)),
    m_current(other.m_current)
{
    other.m_current = nullptr;
}

LinearAllocator::~LinearAllocator() noexcept
{
    Clear();
}

LinearAllocator& LinearAllocator::operator=(LinearAllocator&& rhs) noexcept
{
    Allocator::operator=(std::move(rhs));
    m_current = rhs.m_current;
    rhs.m_current = nullptr;
    return *this;
}

void* LinearAllocator::Allocate(const std::size_t& size,
                                     const std::uintptr_t& alignment)
{
    assert(size > 0 && alignment > 0);

   std::size_t adjustment
        = align_forward_adjustment(m_current, alignment);

    if(m_usedBytes + adjustment + size > m_size)
        throw std::bad_alloc();

    void* alignedAddr = ptr_add(m_current, adjustment);

    m_current = ptr_add(alignedAddr, size);

    m_usedBytes = reinterpret_cast<std::uintptr_t>(m_current)
        - reinterpret_cast<std::uintptr_t>(m_start);

    ++m_numAllocations;


    return alignedAddr;
}

void LinearAllocator::Free([[maybe_unused]] void* const ptr) noexcept
{
    // you can't free from a linear allocator
}


void LinearAllocator::Rewind(void* const mark) noexcept
{
    assert(m_current >= mark && m_start <= mark);

    m_current = mark;

    m_usedBytes = reinterpret_cast<std::uintptr_t>(m_current)
        - reinterpret_cast<std::uintptr_t>(m_start);

}

void LinearAllocator::Clear() noexcept
{
    m_numAllocations = 0;
    m_usedBytes = 0;
    m_current = const_cast<void*>(m_start);
}

void* LinearAllocator::GetCurrent() const noexcept
{
    return m_current;
}





template<class T>
struct Mallocator
{
    typedef T value_type;
 
    Mallocator () = default;
 
    template<class U>
    constexpr Mallocator (const Mallocator <U>&) noexcept {}
 
    [[nodiscard]] T* allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_array_new_length();
 
        if (auto p = static_cast<T*>(std::malloc(n * sizeof(T))))
        {
            report(p, n);
            return p;
        }
 
        throw std::bad_alloc();
    }
 
    void deallocate(T* p, std::size_t n) noexcept
    {
        report(p, n, 0);
        std::free(p);
    }
private:
    void report(T* p, std::size_t n, bool alloc = true) const
    {
        std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }
};

