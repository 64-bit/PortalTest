#pragma once
#include "Color.h"

class Texture2D
{
public:
	int Width;
	int Height;

	Color_b* Pixels;

	Texture2D(Texture2D&& other) noexcept; // move constructor)
	Texture2D();
	Texture2D(int width, int height);
	~Texture2D();

private:

};