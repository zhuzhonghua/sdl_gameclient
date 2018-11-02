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

	class Attribute{
	public:
		Attribute(){ _location = 0; }
		Attribute(GLuint lo) { _location = lo; }
		GLuint handle() { return _location; }
		void enable() { glEnableVertexAttribArray(_location); }
		void disable() { glDisableVertexAttribArray(_location); }

		void vertexPointer(int size, int stride, float* ptr) { glVertexAttribPointer(_location, size, GL_FLOAT, GL_FALSE, stride, (void*)ptr); }
	private:
		GLuint _location;
	};

	class Uniform{
	public:
		Uniform(){ _location = 0; }
		Uniform(GLuint lo) { _location = lo; }
		GLuint handle() { return _location; }

		void enable() { glEnableVertexAttribArray(_location); }
		void disable() { glDisableVertexAttribArray(_location); }

		void value(int v) { glUniform1i(_location, v); }
		void value1f(float v) { glUniform1f(_location, v); }
		void value2f(float v1, float v2) { glUniform2f(_location, v1, v2); }
		void value4f(float v1, float v2, float v3, float v4) { glUniform4f(_location, v1, v2, v3, v4); }
		void valueM4(const Matrix& m) { glUniformMatrix4fv(_location, 1, GL_FALSE, &(m[0][0])); }
	private:
		GLuint _location;
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
	Attribute _aPos;
	Attribute _aUV;
	Uniform _uCamera;
};
}