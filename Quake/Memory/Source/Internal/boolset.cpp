#include "..\..\Include\Internal\boolset.h"

namespace internal
{
	BoolSet::BoolSet()
		: _bools(nullptr), _size(0)
	{
	}

	BoolSet::BoolSet(size_t size, QBool initial)
		: _bools(new QBool[size]), _size(size)
	{
		for (size_t i = 0; i < size; i++)
		{
			_bools[i] = initial;
		}
	}

	BoolSet::BoolSet(const BoolSet& bSet)
		: _bools(new QBool[bSet._size]), _size(bSet._size)
	{	
		internal::copy(bSet._bools, _size, _bools);
	}

	BoolSet::~BoolSet()
	{
		clear();
	}

	void BoolSet::reset()
	{
		for (size_t i = 0; i < _size; i++)
		{
			_bools[i] = false;
		}
	}

	void BoolSet::clear()
	{
		delete[] _bools;
		_bools = nullptr; // Prevent dangling pointer
		_size = 0;
	}

	QBool BoolSet::all() const
	{
		for (size_t i = 0; i < _size; i++)
		{
			if (!_bools[i])
			{
				return false;
			}
		}
		return true;
	}

	QBool BoolSet::any() const
	{
		for (size_t i = 0; i < _size; i++)
		{
			if (_bools[i])
			{
				return true;
			}
		}
		return false;
	}

	QBool BoolSet::none() const
	{
		for (size_t i = 0; i < _size; i++)
		{
			if (_bools[i])
			{
				return false;
			}
		}
		return true;
	}

	void BoolSet::operator=(const BoolSet& bSet)
	{
		if (this != &bSet) // Handle self-assignment
		{
			delete[] _bools; // Free existing memory
			_size = bSet._size;
			_bools = new QBool[_size];
			internal::copy(bSet._bools, _size, _bools);
		}
	}

	QBool& BoolSet::operator[](size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index out of range");
		}
		return _bools[index];
	}

	void BoolSet::resize(size_t amount)
	{
		size_t newSize = _size + amount;
		QBool* newBools = new QBool[newSize];
		for (size_t i = 0; i < _size; i++)
		{
			newBools[i] = _bools[i];
		}
		delete[] _bools; // Free old memory
		_bools = newBools;
		_size = newSize;
	}

	size_t BoolSet::count() const
	{
		size_t count = 0;
		for (size_t i = 0; i < _size; i++)
		{
			if (_bools[i])
			{
				count++;
			}
		}
		return count;
	}

	size_t BoolSet::getSize() const
	{
		return _size;
	}
}