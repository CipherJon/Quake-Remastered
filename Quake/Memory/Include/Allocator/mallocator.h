#pragma once

#include "base_allocator.h"
#include <climits>

#define MALLOCATOR_MAX UINT_MAX

class MAllocator
{
public:
	MAllocator();
	Block allocate(size_t size);
	void deallocate(Block& block);
	void destroy();
	ALLOCATOR_ID getID() const;
private:
	size_t _numAllocations;
};