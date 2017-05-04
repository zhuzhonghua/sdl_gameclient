#include "glslprogram.h"

#include <fstream>
using namespace Zhu;
GLSLProgram::GLSLProgram()
{
	_programID = 0;
	_vertexShaderID = 0;
	_fragmentShaderID = 0;

	_numAttributes = 0;
}

GLSLProgram::~GLSLProgram()
{

}

void GLSLProgram::compileShaders(std::string vertexShaderFilePath, std::string fragmenShaderFilePath)
{
	_programID = glCreateProgram();

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0)
	{
		fatalError("vertex shader create fail");
	}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0)
	{
		fatalError("frag shader create fail");
	}
	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmenShaderFilePath, _fragmentShaderID);
}

void GLSLProgram::compileShader(std::string filePath, GLuint id)
{
	SDL_RWops* fileOP = SDL_RWFromFile(filePath.c_str(), "r");
	if (fileOP == NULL)
	{
		perror(filePath.c_str());
		fatalError("Fail to open " + filePath);
	}

	int len = SDL_RWsize(fileOP);
	
	std::string fileContents;
	fileContents.resize(len + 1);
	SDL_RWread(fileOP, &fileContents[0], len, 1);
	SDL_RWclose(fileOP);

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, NULL);

	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLen = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLen);

		std::vector<char> errLog(maxLen);
		glGetShaderInfoLog(id, maxLen, &maxLen, &errLog[0]);

		glDeleteShader(id);

		std::printf("%s\n", &errLog[0]);
		fatalError("compile shader " + filePath + " fail");
		return;
	}
}

void GLSLProgram::linkShaders()
{
	
	
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	glLinkProgram(_programID);

	GLint success = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&success);
	if (success == GL_FALSE)
	{
		GLint maxLen = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLen);

		std::vector<char> infoLog(maxLen);
		glGetProgramInfoLog(_programID, maxLen, &maxLen, &infoLog[0]);

		glDeleteProgram(_programID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		std::printf("%s\n", &infoLog[0]);
		fatalError("link error");
	}

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);

	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void GLSLProgram::addAttribute(std::string attributeName)
{
	glBindAttribLocation(_programID, _numAttributes, attributeName.c_str());
	_numAttributes++;
}

GLuint GLSLProgram::getUniformLocation(std::string uniformName)
{
	GLuint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX)
	{
		fatalError("Uniform "+uniformName+" not found!");
	}
	return location;
}

void GLSLProgram::use()
{
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse()
{
	glUseProgram(0);

	for (int i = 0; i < _numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}