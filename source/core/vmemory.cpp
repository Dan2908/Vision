#include "include/vmemory.h"

namespace Vision
{
namespace Core
{
//********************************
//     Class MemoryPool
//********************************
//----------------------------------------------------------------
Allocator::MemoryPool::MemoryPool(const std::size_t sizeInBytes) 
    : mMemoryBegin(new Byte[sizeInBytes])
    , mCurrentPtr(0)
    , mAvailableSpace(sizeInBytes)
{}
//----------------------------------------------------------------
Allocator::MemoryPool::~MemoryPool()
{
    delete[] mMemoryBegin;
}

//********************************
//     Class Allocator
//********************************
//----------------------------------------------------------------
Allocator::Allocator()
{}

//----------------------------------------------------------------
Allocator::~Allocator()
{}

//----------------------------------------------------------------
void Allocator::AddMemoryPool()
{
    mMemoryPools.push_back(std::make_unique<MemoryPool>(MemoryPool()));
}

} // namespace Core
} // namespace Vision

	