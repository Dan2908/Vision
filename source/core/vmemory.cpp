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
    MemoryPool* newPool = new MemoryPool();
    mMemoryPools.insert(std::make_pair(newPool->GetAvailableSpace(), newPool));
}

void Allocator::UpdatePoolOrder(PoolMap::iterator& changedPool)
{
    mMemoryPools.insert(std::make_pair(changedPool->second->GetAvailableSpace(), changedPool->second));
    mMemoryPools.erase(changedPool);
}

} // namespace Core
} // namespace Vision

	