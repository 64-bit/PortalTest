#include "stdafx.h"
#include "Texture2D.h"
#include "stb_image.h"
#include <string>

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

Texture2D::Texture2D(FILE* file)
{
	int components;
	Pixels = (Color_b*)stbi_load_from_file(file, &Width, &Height, &components, 4);
}

Color_b Texture2D::Sample(const glm::vec2 uv)
{

	int x = ((int)(uv.x * (float)Width)) % (Width-1);
	int y = ((int)(uv.y * (float)Height)) % (Height-1);

	return Pixels[x + y * Width];
}

Texture2D::~Texture2D()
{
	if (Pixels != nullptr)
	{
		delete[] Pixels;
		Pixels = nullptr;
	}
}

Texture2D* Texture2D::ReadFromFile(char* filepath)
{
	std::string combined = std::string("../Assets/") + filepath;
	FILE* file;
	fopen_s(&file, combined.c_str(), "rb");
	Texture2D* result = new Texture2D(file);
	fclose(file);

	return result;
}
