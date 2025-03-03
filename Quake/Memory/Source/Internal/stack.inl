#include "..\..\Include\Internal\stack.hpp"

namespace internal
{
	template<typename T>
	Stack<T>::Stack(size_t capacity)
		: _stack(nullptr), _capacity(capacity), _size(0)
	{
		_stack = new T[_capacity];
	}

	template<typename T>
	Stack<T>::~Stack(void)
	{
		clear();
	}

	template<typename T>
	void Stack<T>::push(const T& elem)
	{
		if (_size < _capacity)
		{
			_stack[_size] = elem;
			_size++;
		}
	}

	template<typename T>
	void Stack<T>::pop(void)
	{
		if (_size > 0)
		{
			_size--;
			_stack[_size] = T();
		}
	}

	template<typename T>
	void Stack<T>::clear(void)
	{
		delete[] _stack;
		_stack = nullptr;
		_size = 0;
	}

	template<typename T>
	T& Stack<T>::getTop(void)
	{
		if (_size == 0)
		{
			throw std::out_of_range("Stack is empty");
		}
		return _stack[_size - 1];
	}

	template<typename T>
	size_t Stack<T>::getSize(void) const
	{
		return _size;
	}
}