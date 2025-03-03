#pragma once

#include <stdint.h>

/*
==================================

TYPEDEFS

==================================
*/

// Floating-point types
typedef float                           float32;  // 32-bit floating point
typedef double                          float64;  // 64-bit floating point

// Character types
typedef char                            Char8;    // 8-bit character
typedef char16_t                        Char16;   // 16-bit character
typedef char32_t                        Char32;   // 32-bit character
typedef wchar_t                         WChar;    // Wide character

// Integer types
typedef int8_t                          int8;     // 8-bit signed integer
typedef int32_t                         int32;    // 32-bit signed integer
typedef int64_t                         int64;    // 64-bit signed integer
typedef uint32_t                        uint32;   // 32-bit unsigned integer
typedef uint64_t                        uint64;   // 64-bit unsigned integer

// Size type
typedef uint64_t                        size64;   // 64-bit size type

// Window dimensions
#define WINDOW_WIDTH        800
#define WINDOW_HEIGHT       600