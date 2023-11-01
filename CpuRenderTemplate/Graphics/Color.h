#pragma once

struct Color_f;

struct Color_b
{
public:
	char r;
	char g;
	char b;
	char a;

	inline Color_b();

	inline Color_b (char R, char G, char B, char A);

	inline Color_b(Color_f color);
};

struct Color_f
{
public:
	float r;
	float g;
	float b;
	float a;

	inline Color_f();

	inline Color_f(float R, float G, float B, float A);

	inline Color_f(Color_b color);
};

inline Color_f::Color_f()
{
}

inline Color_f::Color_f(float R, float G, float B, float A)
{
	r = R;
	g = G;
	b = B;
	a = A;
}

inline Color_f::Color_f(Color_b color)
{
	r = (float)color.r / 255.0f;
	g = (float)color.g / 255.0f;
	b = (float)color.b / 255.0f;
	a = (float)color.a / 255.0f;
}

inline Color_b::Color_b()
{
}

inline Color_b::Color_b(char R, char G, char B, char A)
{
	r = R;
	g = G;
	b = B;
	a = A;
}

inline Color_b::Color_b(Color_f color)
{
	r = (int)(color.r * 255.0f);
	g = (int)(color.r * 255.0f);
	b = (int)(color.r * 255.0f);
	a = (int)(color.r * 255.0f);
}
