
// #include "allocator.hpp"

template<typename T, typename Alloc>
class STLAdaptor
{
public:

    typedef T value_type; 

    STLAdaptor() = delete;

    STLAdaptor(Alloc& allocator) noexcept
    :
        m_allocator(allocator)
    {

    }

    template<typename U>
    STLAdaptor(const STLAdaptor<U, Alloc>& other) noexcept
    :
        m_allocator(other.m_allocator)
    {}

    [[nodiscard]] constexpr T* allocate(std::size_t n)
    {
        return reinterpret_cast<T*>
            (m_allocator.Allocate(n * sizeof(T), alignof(T)));
    }

    constexpr void deallocate(T* p, [[maybe_unused]] std::size_t n)
    noexcept
    {
        m_allocator.Free(p);
    }

    std::size_t MaxAllocationSize() const noexcept
    {
        return m_allocator.GetSize();
    }

    bool operator==(const STLAdaptor<T,Alloc>& rhs) const noexcept
    {
        if constexpr(std::is_base_of_v<FixedAllocator,Alloc>)
        {
            return m_allocator.GetStart() == rhs.m_allocator.GetStart();
        }
        else
        {
            DynamicAllocator::BlockDesc* a =
                reinterpret_cast<DynamicAllocator*>
                (&m_allocator)->m_currentBlock;

            while(a->prevBlock != nullptr)
            {
                a = a->prevBlock;
            }

            DynamicAllocator::BlockDesc* b =
                reinterpret_cast<DynamicAllocator*>
                (&rhs.m_allocator)->m_currentBlock;

            while(b->prevBlock != nullptr)
            {
                b = b->prevBlock;
            }

            return a == b;
        }
    }

    bool operator!=(const STLAdaptor<T,Alloc>& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    Alloc& m_allocator; 
};