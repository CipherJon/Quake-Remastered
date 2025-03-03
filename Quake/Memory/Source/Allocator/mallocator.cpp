#include "..\..\Include\Allocator\mallocator.h"

MAllocator::MAllocator()
	: _numAllocations(0)
{
}

Block MAllocator::allocate(size_t size)
{
	void* mem = malloc(size);
	if (mem)
	{
		Block block(mem, size);
		_numAllocations++;
		return block;
	}
	return Block(nullptr, 0);
}

void MAllocator::deallocate(Block& block)
{
	if (_numAllocations && block.memory)
	{
		free(block.memory);
		block.memory = nullptr;
		block.length = 0;
		_numAllocations--;
	}
}

void MAllocator::destroy()
{
	// Implement any necessary cleanup here
}

ALLOCATOR_ID MAllocator::getID() const
{
	return ALLOCATOR_ID::MALLOCATOR;
}