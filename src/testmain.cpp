#include "stdafx.h"

struct Position
{
	float x;
	float y;
};

struct Color
{
	Color() :r(255), g(255), b(255), a(255){}
	Color(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
		r(R), g(G), b(B), a(A){}
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct UV
{
	float u;
	float v;
};

struct Vertex
{
	Position position;

	Color color;

	UV uv;

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
	void setPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
	void setUV(float u, float v)
	{
		uv.u = u;
		uv.v = v;
	}
};

int main(int argc, char** args)
{
	const int SCREEN_WIDTH = 550;
	const int SCREEN_HEIGHT = 400;

	// Init Window
	SDL_Window* gWindow = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	gWindow = SDL_CreateWindow("MyGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(gWindow);
	SDL_GL_SetSwapInterval(1);

	glewInit();

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Init Texture
	SDL_Surface* img = IMG_Load("data/ninja/bricks_top.jpg");

	int width = img->w;
	int height = img->h;

	int x = 0;
	int y = 10;

	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Init VertexBuffer
	GLuint vboID;
	glGenBuffers(1, &vboID);
	Vertex vertexData[6];
	vertexData[0].setPosition(x + width, y + height);
	vertexData[0].setUV(1.0f, 1.0f);

	vertexData[1].setPosition(x, y + height);
	vertexData[1].setUV(0, 1.0f);
	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0, 0);
	vertexData[3].setPosition(x, y);
	vertexData[3].setUV(0, 0);
	vertexData[4].setPosition(x + width, y);
	vertexData[4].setUV(1, 0);
	vertexData[5].setPosition(x + width, y + height);
	vertexData[5].setUV(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// Init Shader
	GLuint programID = glCreateProgram();
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	SDL_RWops* fileOP = SDL_RWFromFile("data/ninja/test.vert", "r");
	int len = SDL_RWsize(fileOP);
	std::string fileContents;
	fileContents.resize(len + 1);
	SDL_RWread(fileOP, &fileContents[0], len, 1);
	SDL_RWclose(fileOP);

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(vertexShaderID, 1, &contentsPtr, NULL);
	glCompileShader(vertexShaderID);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	fileOP = SDL_RWFromFile("data/ninja/test.frag", "r");
	len = SDL_RWsize(fileOP);
	fileContents.resize(len + 1);
	SDL_RWread(fileOP, &fileContents[0], len, 1);
	SDL_RWclose(fileOP);

	contentsPtr = fileContents.c_str();
	glShaderSource(fragmentShaderID, 1, &contentsPtr, NULL);
	glCompileShader(fragmentShaderID);

	glBindAttribLocation(programID, 0, "vertexPosition");
	glBindAttribLocation(programID, 1, "vertexColor");
	glBindAttribLocation(programID, 2, "vertexUV");

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Draw
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(programID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	GLuint location = glGetUniformLocation(programID, "mySampler");
	glUniform1i(location, 0);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texId);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnableClientState(GL_COLOR_ARRAY);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0, 0);
	glVertex2f(0, 500);
	glVertex2f(500, 500);
	glEnd();

	SDL_GL_SwapWindow(gWindow);
	SDL_Delay(4000);

	SDL_FreeSurface(img);
	SDL_DestroyWindow(gWindow);

	SDL_Quit();

	return 0;
}