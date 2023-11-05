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

	Texture2D(FILE* file);

	Color_b Sample(const glm::vec2 uv);

	~Texture2D();

	static Texture2D* ReadFromFile(char* filepath);

private:

};