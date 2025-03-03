#pragma once

/// Simple C-style string container
struct CString
{
	char* string;     // Null-terminated string buffer
	size_t size;      // Current string length
	size_t capacity;  // Allocated buffer size
};

/// Generic C-style array container
template<typename Type>
struct CArray
{
	Type* array;      // Array buffer
	size_t size;      // Current number of elements
	size_t capacity;  // Allocated buffer size
};