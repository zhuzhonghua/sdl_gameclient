#pragma once

namespace Zengine{
class GLSLProgram{
public:
	struct Position{
		float x;
		float y;
	};

	struct UV{
		float u;
		float v;
	};

	struct Vertex{
		Position pos;
		UV uv;

		void setPos(float x, float y) { pos.x = x, pos.y = y; }
		void setUV(float u, float v) { uv.u = u, uv.v = v; }
	};
public:
	static GLSLProgram* inst();
	GLSLProgram();

	void init();
	void use();
	void drawQuad(Vertex* buffer);

	GLuint programID() { return _programID; }
protected:
	static GLSLProgram* _inst;

	void compileShader(GLuint shaderID, const char* src);
	void link(GLuint programID);
	void loadShaders();

	GLuint _programID;
	GLuint _vertexShaderID;
	GLuint _fragShaderID;
};
}