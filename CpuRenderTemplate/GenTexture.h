#pragma once
#include "Graphics/Texture2D.h"

__inline void GenerateTexture(Texture2D& target, int offset)
{

	int pixels = target.Width * target.Height;
	int* colorPtr = reinterpret_cast<int*>(target.Pixels);

	int l = 0xFF0000FF;
	int r = 0x0000FFFF;

	for (int i = 0; i < pixels; i++)
	{
		int x = i % target.Width;
		int y = i / target.Width;

		x /= 32;
		x += offset;
		y /= 32;

		bool isOdd = x % 2;
		isOdd ^= y % 2;

		colorPtr[i] = isOdd ? l : r;	
	}
}