#pragma once
#include <GL/glew.h>

class FrameDisplayer
{
public:
	FrameDisplayer();

	void DisplayFrame(int width, int height, char* data);

private:
	int _currentWidth;
	int _currentHeight;

	GLuint vao, vbo, ebo, tex, shaderProg;

	GLuint CompileShaderProgram();

	GLuint CreateTexture();

};