#pragma once


__inline char* GenerateTexture(int width, int height, int offset) 
{
	int pixels = width * height;
	char* data = new char[pixels * 4];
	int* colorPtr = reinterpret_cast<int*>(data);

	int l = 0xFF0000FF;
	int r = 0x0000FFFF;

	for (int i = 0; i < pixels; i++)
	{
		int x = i % width;
		int y = i / width;

		x /= 32;
		x += offset;
		y /= 32;

		bool isOdd = x % 2;
		isOdd ^= y % 2;

		colorPtr[i] = isOdd ? l : r;	
	}

	return data;
}