#include "stdafx.h"
#include "Texture2D.h"

Texture2D::Texture2D(Texture2D&& other) noexcept
{
	Width = std::move(other.Width);
	Height = std::move(other.Height);

	Pixels = std::move(other.Pixels);
}

Texture2D::Texture2D()
{
	Pixels = nullptr;
	Width = 0;
	Height = 0;
}

Texture2D::Texture2D(int width, int height)
{
	Width = width;
	Height = height;

	Pixels = new Color_b[width * height];
}

Texture2D::~Texture2D()
{
	if (Pixels != nullptr)
	{
		delete[] Pixels;
		Pixels = nullptr;
	}
}
