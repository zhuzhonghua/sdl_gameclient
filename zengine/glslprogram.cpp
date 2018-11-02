#include "stdafx.h"

#include "glslprogram.h"
#include "camera2d.h"

namespace Zengine{

const char* VERT_SHADER_SRC = R"(
attribute vec2 aPos;
attribute vec2 aUV;

uniform mat4 uCamera;

varying vec2 vUV;

void main(){
	gl_Position.xy = (uCamera * vec4(aPos, 0, 1)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	vUV = aUV;
}
)";

const char* FRAG_SHADER_SRC = R"(
varying vec2 vUV;
uniform sampler2D uTex;

void main(){
	gl_FragColor = texture2D(uTex, vUV);
}
)";

void GLSLProgram::compileShader(GLuint shaderID, const char* src)
{
	glShaderSource(shaderID, 1, &src, NULL);
	glCompileShader(shaderID);

	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLen = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLen);

		std::vector<char> errLog(maxLen);
		glGetShaderInfoLog(shaderID, maxLen, &maxLen, &errLog[0]);

		glDeleteShader(shaderID);

		std::printf("%s\n", &errLog[0]);
		fatalError("compile shader fail");
	}
}

void GLSLProgram::link(GLuint programID)
{
	glLinkProgram(programID);

	GLint success = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, (int*)&success);
	if (success == GL_FALSE)
	{
		GLint maxLen = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLen);

		std::vector<char> infoLog(maxLen);
		glGetProgramInfoLog(programID, maxLen, &maxLen, &infoLog[0]);

		glDeleteProgram(programID);

		std::printf("%s\n", &infoLog[0]);
		fatalError("link error");
	}
}

void GLSLProgram::loadShaders()
{
	_programID = glCreateProgram();
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	compileShader(_vertexShaderID, VERT_SHADER_SRC);
	glAttachShader(_programID, _vertexShaderID);

	compileShader(_fragShaderID, FRAG_SHADER_SRC);
	glAttachShader(_programID, _fragShaderID);

	link(_programID);
}

GLSLProgram::GLSLProgram()
{
	ASSERT(_inst == NULL);
	_inst = this;
	loadShaders();

	int attr = 0;
	glBindAttribLocation(_programID, attr, "aPos");
	_aPos = Attribute(attr++);
	glBindAttribLocation(_programID, attr, "aUV");
	_aUV = Attribute(attr++);
	
	_uCamera = Uniform(glGetUniformLocation(_programID, "uCamera"));
}

GLSLProgram* GLSLProgram::_inst;
GLSLProgram* GLSLProgram::inst()
{
	if (_inst == NULL)
	{
		new GLSLProgram();
	}

	return _inst;
}

void GLSLProgram::init()
{
	// do nothing
}

void GLSLProgram::use()
{
	glUseProgram(_programID);

	_aPos.enable();
	_aUV.enable();
}

void GLSLProgram::drawQuad(Vertex* buffer)
{
	_uCamera.valueM4(Camera2D::mainCamera->cameraMatrix);

	char* ptr = (char*)buffer;
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(ptr + offsetof(Vertex, pos)));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(ptr + offsetof(Vertex, uv)));

	short indices[] = { 0, 1, 2, 0, 2, 3 };
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)indices);
}
}