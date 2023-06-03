#pragma once

#include <assert.h>
#include <algorithm>
#include <common/include/common.h>
#include <memory>
#include <vector>

namespace Vision
{
namespace Core
{
// TODO: optimize this size based on benchmarking (when there is such)
static const std::size_t DEFAULT_POOL_SIZE = 1024 * 4;

class Allocator
{
    class MemoryPool
    {
        using Byte = unsigned char;
        Byte* mMemoryBegin;
        Byte* mCurrentPtr;
        size_t mAvailableSpace;
        size_t mCapacity;
        
        inline Byte* Assign(const std::size_t count)
        {
            assert(count <= mAvailableSpace);
            mCurrentPtr += count;
            mAvailableSpace -= count;
            return mCurrentPtr - count;
        }

    public:
        MemoryPool(const std::size_t sizeInBytes = DEFAULT_POOL_SIZE);
        ~MemoryPool();

        friend inline const bool operator<(const MemoryPool a, const MemoryPool b)
        {
            return (a.mAvailableSpace < b.mAvailableSpace);
        }
        /* Function to sort pools by space (using pointer required for this container)*/
        friend inline const bool LessComparer(const MemoryPool* a, const MemoryPool* b)
        {
            return ((*a) < (*b));
        }

        template<class T>
        T* Allocate(const std::size_t count)
        {
            if (count > mAvailableSpace)
            {
                return nullptr;
            }
            return reinterpret_cast<T*>(Assign(count * sizeof(T)));
        }

        template<class T>
        inline size_t GetSize() { return mCapacity / sizeof(T); }
        /* Returns available space in bytes for this pool. */
        inline size_t GetAvailableSpace() { return mAvailableSpace; }
        /* Returns total space in T for this pool. */
        inline const size_t Capacity() { return mCapacity; }
    };

    std::vector<std::unique_ptr<MemoryPool>> mMemoryPools;

    void AddMemoryPool();

public:
    Allocator();
    ~Allocator();

    template<class T>
    T* Allocate(const size_t count)
    {
        // Sorted vector, selecting the most space available
        MemoryPool* target = mMemoryPools.back().get();

        const std::size_t dataSize = count * sizeof(T);

        if (dataSize > target->GetAvailableSpace())
        {
            AddMemoryPool();
            return Allocate<T>(count);//recurse
        }

        T* output = target->Allocate<T>(count);
        // Reorder vector based on available space
        std::sort(mMemoryPools.begin(), mMemoryPools.end());

        return output;
    }

    template<class T>
    T* AllocateAndInsert(const T& object)
    {
        T* target = Allocate<T>(1);

        assert(target != nullptr);

        std::memcpy(target, &object, sizeof(object));
        return target;
    }

    template<class T>
    T* AllocateAndInsert(std::initializer_list<T> objects)
    {
        T* target = Allocate<T>(objects.size());
        for (const auto& object : objects)
        {
            *target = object;
            ++target;
        }

        return (target - objects.size());
    }
};

} //namespace Core
} //namespace Vision