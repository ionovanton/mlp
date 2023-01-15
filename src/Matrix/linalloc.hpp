#pragma once
#ifndef LINALLOC_HPP
#define LINALLOC_HPP

#include "allocator.hpp"

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
 
template<class T, class U>
bool operator==(const Mallocator <T>&, const Mallocator <U>&) { return true; }
 
template<class T, class U>
bool operator!=(const Mallocator <T>&, const Mallocator <U>&) { return false; }

#endif