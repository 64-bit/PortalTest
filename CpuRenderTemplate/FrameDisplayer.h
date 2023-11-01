#pragma once
#include <GL/glew.h>

class Texture2D;

class FrameDisplayer
{
public:
	FrameDisplayer();

	void DisplayFrame(Texture2D* data);

private:
	int _currentWidth;
	int _currentHeight;

	GLuint vao, vbo, ebo, tex, shaderProg;

	GLuint CompileShaderProgram();

	GLuint CreateTexture();

};