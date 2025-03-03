#include "..\..\Include\Allocator\pool_allocator.hpp"

template<typename TBlock>
PoolAllocator<TBlock>::PoolAllocator(size_t capacity)
	: _capacity(0), _memoryUsed(0), _numBlocks(0), _numAllocated(0),
	_memory(nullptr), _freeList(), _isInitialized(false)
{
	_trueSize = sizeof(TBlock);
	_padding = (!internal::isPowerOfTwo(_trueSize) ? internal::alignToPowerOfTwo(_trueSize) - _trueSize : 0);
	_blockSize = _trueSize + _padding;

	if (capacity != DO_NOT_INIT_SIZE)
	{
		init(capacity);
	}
}

template<typename TBlock>
PoolAllocator<TBlock>::~PoolAllocator()
{
	destroy();
}

template<typename TBlock>
Block PoolAllocator<TBlock>::allocate()
{
	if (_isInitialized && 
		_hasSpace(_blockSize) &&
		_freeList.hasSpaceFor(1))
	{
		_addMemoryUsed(_blockSize);
		_incrementNumAllocated();
		return _freeList.deallocate();
	}
	return Block(nullptr, 0);
}

template<typename TBlock>
void PoolAllocator<TBlock>::deallocate(Block& block)
{
	if (_isInitialized && owns(block))
	{
		_freeList.allocate(block);
		block.memory = nullptr;
		block.length = 0;
		_subtractMemoryUsed(_blockSize);
		_decrementNumAllocated();
	}
}

template<typename TBlock>
Batch PoolAllocator<TBlock>::batchAllocate(size_t size)
{
	size_t allocSize = size * _blockSize;
	if (_isInitialized && _hasSpace(allocSize))
	{
		size_t numToAlloc = size;
		if (_freeList.hasNumFree(numToAlloc))
		{
			Block* blocks = new Block[numToAlloc];
			for (size_t i = 0; i < numToAlloc; i++)
			{
				blocks[i] = _freeList.deallocate();
			}
			_addMemoryUsed(allocSize);
			_addNumAllocated(numToAlloc);
			return Batch(blocks, size);
		}
	}
	return Batch(nullptr, 0);
}

template<typename TBlock>
void PoolAllocator<TBlock>::batchDeallocate(Batch& batch)
{
	size_t size = batch.size;
	if (_isInitialized && batch.blocks && _freeList.hasSpaceFor(size))
	{
		size_t allocSize = 0;
		for (size_t i = 0; i < size; i++)
		{
			if (_freeList.owns(batch.blocks[i]))
			{
				_freeList.allocate(batch.blocks[i]);
				allocSize += _blockSize;
			}
		}
		delete[] batch.blocks;
		batch.blocks = nullptr;
		_subtractMemoryUsed(allocSize);
		_subtractNumAllocated(size);
	}
}

template<typename TBlock>
QBool PoolAllocator<TBlock>::owns(Block block) const
{
	return _isInitialized && _freeList.owns(block);
}

template<typename TBlock>
ALLOCATOR_INIT_STATUS PoolAllocator<TBlock>::init(size_t capacity)
{
	ALLOCATOR_INIT_STATUS status = ALLOCATOR_INIT_STATUS::NONE;
	size_t alignedCapacity = internal::alignToPowerOfTwo(capacity) * _blockSize;
	size_t sizeMax = std::numeric_limits<size_t>::max();
	QBool capacityLimit = alignedCapacity <= sizeMax && alignedCapacity != 0;
	if (!_isInitialized && capacityLimit)
	{
		_capacity = alignedCapacity;
		_freeList.init(capacity, _blockSize);
		if (_freeList.isInitialized())
		{
			_numBlocks = _capacity / _blockSize;
			size_t numBytes = _capacity / BYTE_SIZE;
			_memory = new internal::Byte[numBytes];

			internal::Byte* itr = _memory;
			if (itr)
			{
				for (size_t i = 0; i < _numBlocks; i++)
				{
					Block block(static_cast<void*>(itr), _blockSize);
					_freeList.allocate(block);
					itr += _blockSize;
				}
				_isInitialized = true;
				return ALLOCATOR_INIT_STATUS::SUCCESS;
			}
			else
			{
				return ALLOCATOR_INIT_STATUS::BAD_MEMORY;
			}
		}
		else
		{
			return ALLOCATOR_INIT_STATUS::INTERNAL_ERROR;
		}
	}
	else
	{
		return ALLOCATOR_INIT_STATUS::ALREADY_INITIALIZED;
	}
}

template<typename TBlock>
void PoolAllocator<TBlock>::destroy()
{
	delete[] _memory;
	_memory = nullptr;
	_isInitialized = false;
}

template<typename TBlock>
size_t PoolAllocator<TBlock>::getCapacity() const
{
	return _capacity;
}

template<typename TBlock>
size_t PoolAllocator<TBlock>::getMemoryUsed() const
{
	return _memoryUsed;
}

template<typename TBlock>
size_t PoolAllocator<TBlock>::getBlockSize() const
{
	return _blockSize;
}

template<typename TBlock>
size_t PoolAllocator<TBlock>::getPadding() const
{
	return _padding;
}

template<typename TBlock>
size_t PoolAllocator<TBlock>::getTrueSize() const
{
	return _trueSize;
}

template<typename TBlock>
size_t PoolAllocator<TBlock>::getNumBlocks() const
{
	return _numBlocks;
}

template<typename TBlock>
size_t PoolAllocator<TBlock>::getNumAllocated() const
{
	return _numAllocated;
}

template<typename TBlock>
QBool PoolAllocator<TBlock>::isInitialized() const
{
	return _isInitialized && _freeList.isInitialized();
}

template<typename TBlock>
QBool PoolAllocator<TBlock>::isFull() const
{
	return _memoryUsed == _capacity;
}

template<typename TBlock>
QBool PoolAllocator<TBlock>::hasFreeMemory() const
{
	return _memoryUsed < _capacity;
}

template<typename TBlock>
ALLOCATOR_ID PoolAllocator<TBlock>::getID() const
{
	return ALLOCATOR_ID::POOL;
}

template<typename TBlock>
void PoolAllocator<TBlock>::_addMemoryUsed(size_t amount)
{
	_memoryUsed += amount;
}

template<typename TBlock>
void PoolAllocator<TBlock>::_subtractMemoryUsed(size_t amount)
{
	_memoryUsed -= amount;
}

template<typename TBlock>
void PoolAllocator<TBlock>::_incrementNumAllocated()
{
	_numAllocated++;
}

template<typename TBlock>
void PoolAllocator<TBlock>::_decrementNumAllocated()
{
	_numAllocated--;
}

template<typename TBlock>
void PoolAllocator<TBlock>::_addNumAllocated(size_t amount)
{
	_numAllocated += amount;
}

template<typename TBlock>
void PoolAllocator<TBlock>::_subtractNumAllocated(size_t amount)
{
	_numAllocated -= amount;
}

template<typename TBlock>
QBool PoolAllocator<TBlock>::_hasSpace(size_t addedSize)
{
	return _memoryUsed + addedSize <= _capacity;
}