#pragma once

#include "glad\glad.h"
#include "GLFW\glfw3.h"

class Program
{
public:
	Program(const char* vertextShaderSrcFile, const char* fragmentShaderSrcFile);
	~Program();

	void useProgram()
	{
		glUseProgram(shaderProgram);
	};

	unsigned int getVertexShaderID() { return vertexShader; }
	unsigned int getFragmentShaderID() { return fragmentShader; }
	unsigned int id() { return shaderProgram; }

private:
	unsigned int shaderProgram{ 0 };
	unsigned int vertexShader{ 0 };
	unsigned int fragmentShader{ 0 };

};