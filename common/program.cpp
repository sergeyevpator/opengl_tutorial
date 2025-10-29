#include "program.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <functional>


using namespace std;
using namespace std::filesystem;


Program::Program(const char* vertextShaderSrcFile, const char* fragmentShaderSrcFile)
{
    char infoLog[512];

    uintmax_t fSize;
    unsigned int sCount = 5;
    char** inBuf;

    //creating vertex shader
	ifstream ifs(vertextShaderSrcFile, ios_base::in);
	fSize = file_size(vertextShaderSrcFile);

    inBuf = new char*[sCount];
    for (unsigned int i = 0; i < sCount; i++)
        inBuf[i] = new char[fSize+1]();

    unique_ptr<char*, std::function<void(char**)>> inBufP(inBuf,
        [sCount](char** x)->void {
            for (unsigned int i = 0; i < sCount; i++)
                delete x[i];
            delete [] x;
        });

    ifs.read(inBufP.get()[0], fSize);

	ifs.close();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, inBufP.get(), NULL);
    glCompileShader(vertexShader);

    int  success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    inBufP.reset();

    //creating fragment shader
    ifs.open(fragmentShaderSrcFile, ios_base::in);
    fSize = file_size(fragmentShaderSrcFile);

    inBuf = new char* [sCount];
    for (unsigned int i = 0; i < sCount; i++)
        inBuf[i] = new char[fSize + 1]();

    inBufP.reset(inBuf);

    ifs.read(inBufP.get()[0], fSize);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, inBufP.get(), NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //creating program
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Program::~Program()
{
    glDeleteProgram(shaderProgram);
}
