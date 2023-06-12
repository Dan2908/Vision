#pragma once

#include <common/include/common.h>
#include <memory>

namespace Vision
{
namespace Core
{
// TODO: optimize this size based on benchmarking (when there is such)
static const std::size_t DEFAULT_POOL_SIZE = 1024 * 4;

// TODO: implement later
template<class T>
class Allocator : std::allocator<T>
{};


} //namespace Core
} //namespace Vision