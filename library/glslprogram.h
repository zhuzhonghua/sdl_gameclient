#pragma once

#include "stdafx.h"
#include<string>

namespace Zhu{
	class GLSLProgram
	{
	protected:
		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
		int _numAttributes;

		void compileShader(const char* source, const std::string& name, GLuint id);
	public:
		GLSLProgram();
		~GLSLProgram();

		void dispose();
		void compileShadersFromSource(const char* vertexSource, const char* fragSource);
		void compileShaders(std::string vertexShaderFilePath, std::string fragmenShaderFilePath);
		void linkShaders();
		void addAttribute(std::string attributeName);
		GLuint getUniformLocation(std::string uniformName);

		void use();
		void unuse();
	};
}