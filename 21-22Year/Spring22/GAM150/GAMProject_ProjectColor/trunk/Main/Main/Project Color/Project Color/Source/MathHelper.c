#include "stdafx.h"		// Should be included first in every .c file!
#include "MathHelper.h"

// Lerps between A and B by T.
float Lerp(float a, float b, float t)
{
	return a + ((b - a) * t);
}

// Converts a polar equation to a cartesian equation.
void PolarToCartesian(float theta, float r, float* x, float* y)
{
	*x = r * cosf(theta);
	*y = r * sinf(theta);
}
