#include "..\..\Include\Allocator\freelist.hpp"

FreeList::FreeList()
	: _root(nullptr), _capacity(0), 
	_blockSize(0), _numAllocated(0), _numNodes(0), 
	_memoryUsed(0), _isInitialized(false)
{
}

FreeList::FreeList(size_t capacity,size_t blockSize)
	: _root(nullptr), _capacity(0), 
	_blockSize(0), _numAllocated(0), _numNodes(0), 
	_memoryUsed(0), _isInitialized(false)
{
	init(capacity, blockSize);
}

void FreeList::allocate(Block& block)
{
	if (_isInitialized && !isFull() && _supports(block))
	{
		Node* node = reinterpret_cast<Node*>(block.memory);
		_setNext(node, _root);
		_root = node;
		_incrementNumAllocated();
		_addMemoryUsed();
	}
}

Block FreeList::deallocate()
{
	if (_isInitialized && hasFreeNodes())
	{
		Node* node = _root;
		_root = _getNext(_root);
		void* memory = static_cast<void*>(node);
		Block block(memory, _blockSize);
		_decrementNumAllocated();
		_subtractMemoryUsed();
		return block;
	}
	return Block(nullptr, 0);
}

void FreeList::init(size_t capacity, size_t blockSize)
{
	size_t sizeLimit = std::numeric_limits<size_t>::max();
	size_t alignedBlock = internal::alignToPowerOfTwo(blockSize);
	size_t alignedCapacity = internal::alignToPowerOfTwo(capacity) * alignedBlock;

	QBool blockLimit = alignedBlock <= sizeLimit;
	QBool capacityLimit = alignedCapacity <= sizeLimit;
	if (!_isInitialized && blockLimit && capacityLimit)
	{
		_blockSize = alignedBlock;
		_capacity = alignedCapacity;
		_numNodes = _capacity / _blockSize;
		_isInitialized = true;
	}
}

QBool FreeList::owns(Block block) const
{
	return _isInitialized &&
		block.length == _blockSize;
}

QBool FreeList::isFull() const
{
	return _memoryUsed == _capacity;
}

QBool FreeList::hasSpaceFor(size_t num) const
{
	return (_capacity - _memoryUsed) >= (num * _blockSize);
}

QBool FreeList::hasNumFree(size_t num) const
{
	return (_numNodes - _numAllocated) >= num;
}

QBool FreeList::hasFreeNodes() const
{
	return _numAllocated > 0;
}

size_t FreeList::getCapacity() const
{
	return _capacity;
}

size_t FreeList::getNumAllocated() const
{
	return _numAllocated;
}

size_t FreeList::getNumNodes() const
{
	return _numNodes;
}

size_t FreeList::getMemoryUsed() const
{
	return _memoryUsed;
}

size_t FreeList::getBlockSize() const
{
	return _blockSize;
}

QBool FreeList::isInitialized() const
{
	return _isInitialized;
}

ALLOCATOR_ID FreeList::getID() const
{
	return ALLOCATOR_ID::FREELIST;
}

FreeList::Node* FreeList::_getNext(Node* current)
{
	return current->next;
}

void FreeList::_setNext(Node* current, Node* next)
{
	current->next = next;
}

void FreeList::_incrementNumAllocated()
{
	_numAllocated++;
}

void FreeList::_decrementNumAllocated()
{
	_numAllocated--;
}

void FreeList::_addMemoryUsed()
{
	_memoryUsed += _blockSize;
}

void FreeList::_subtractMemoryUsed()
{
	_memoryUsed -= _blockSize;
}

QBool FreeList::_supports(Block block)
{
	return block.length == _blockSize;
}