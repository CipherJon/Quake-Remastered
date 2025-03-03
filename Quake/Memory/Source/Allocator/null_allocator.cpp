#include "..\..\Include\Allocator\null_allocator.h"

Block NullAllocator::allocate(size_t size)
{
	return Block(nullptr, 0);
}

void NullAllocator::deallocate(Block& block)
{
}

QBool NullAllocator::owns(Block block) const
{
	return false;
}

ALLOCATOR_ID NullAllocator::getID() const
{
	return ALLOCATOR_ID::NULL_ALLOCATOR;
}