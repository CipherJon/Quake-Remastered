#pragma once

#include "..\..\common.h"

#define PI 3.14159265358979323846

namespace math
{
	inline float64 abs(float64 x)
	{
		return((x < 0) ? -x : x);
	}

	inline float64 pow(float64 x, int32 p)
	{
		if (p == 0) return 1;
		float64 n = 1;
		int32 abs_p = math::abs(p);
		for (int32 i = 0; i < abs_p; i++)
		{
			n *= x;
		}
		return (p > 0) ? n : 1 / n;
	}

	inline float64 squared(float64 x)
	{
		return(x * x);
	}

	inline int32 log2(int32 val)
	{
		int32 answer = 0;
		while (val >>= 1)
		{
			answer++;
		}
		return(answer);
	}

	inline int32 GCD(int32 one, int32 two)
	{
		if (one > two)
		{
			return((two == 0) ? one : GCD(two, one % two));
		}
		else
		{
			return((one == 0) ? two : GCD(one, two % one));
		}
	}

	inline float64 sqrt(float64 n)
	{
		return ::sqrt(n); // Use the standard library sqrt function
	}
}