#include "stdafx.h"
#include "png.h"
#include <vector>

struct ImageSource{
	unsigned char* source;
	int len;
	int offset;
};

void readPngData(png_structp png_ptr, png_bytep outBytes,png_size_t byteCountToRead)
{
	png_voidp io_ptr = png_get_io_ptr(png_ptr);
	if (io_ptr == NULL)
	{
		fatalError("png_get_ip_ptr fail");
	}
	ImageSource* source = (ImageSource*)io_ptr;

	if ((int)(source->offset + byteCountToRead) <= source->len)
	{
		memcpy(outBytes, source->source + source->offset, byteCountToRead);
		source->offset += byteCountToRead;
	}
	else
	{
		fatalError("read fail");
	}
}

void _getBytesPNG(const std::string& bitmap, std::vector<unsigned char>& data, unsigned long &width, unsigned long &height)
{
	//This is the input data to decodePNG, which we load from a file
	std::vector<unsigned char> in;
	SDL_RWops* fileOP = SDL_RWFromFile(bitmap.c_str(), "r");
	if (fileOP == NULL)
	{
		fatalError(bitmap.c_str());
	}
	int size = SDL_RWsize(fileOP);
	in.resize(size);
	SDL_RWread(fileOP, &in[0], size, 1);
	SDL_RWclose(fileOP);

	enum { kPngSignatureLength = 8 };
	unsigned char pngSignature[kPngSignatureLength];	
	memcpy(pngSignature, &in[0], kPngSignatureLength);

	if (!png_check_sig(pngSignature, kPngSignatureLength))
	{
		fatalError("png sign error");
	}

	ImageSource src;
	src.source = &in[0];
	src.len = size;
	src.offset = 8;

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_set_read_fn(png_ptr, &src, readPngData);

	png_set_sig_bytes(png_ptr, kPngSignatureLength);

	png_read_info(png_ptr, info_ptr);

	png_uint_32 widthf = 0;
	png_uint_32 heightf = 0;
	int bitDepth = 0;
	int colorType = -1;
	png_uint_32 retval = png_get_IHDR(png_ptr, info_ptr,
		&widthf,
		&heightf,
		&bitDepth,
		&colorType,
		NULL, NULL, NULL);

	if (retval != 1)
	{
		fatalError("png_get_IHDR fail");
	}
	width = widthf;
	height = heightf;

	if (colorType == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png_ptr);
	}
	// low-bit-depth grayscale images are to be expanded to 8 bits
	if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
	{
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}
	// expand any tRNS chunk data into a full alpha channel
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
	}

	if (bitDepth == 16)
	{
		png_set_strip_16(png_ptr);
	}
	// expand grayscale images to RGB
	if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(png_ptr);
	}

	bitDepth = 8;
	png_uint_32 rowbytes;
	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep)* height);

	png_read_update_info(png_ptr, info_ptr);

	rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	data.resize(rowbytes * height);
	unsigned char* pData = &data[0];

	for (unsigned short i = 0; i < height; ++i)
	{
		row_pointers[i] = pData + i*rowbytes;
	}
	png_read_image(png_ptr, row_pointers);

	png_read_end(png_ptr, NULL);

	//png_uint_32 channel = rowbytes / width;
	//if (channel == 4)
	//{
	//	m_bHasAlpha = true;
	//	unsigned int *tmp = (unsigned int *)pData;
	//	for (unsigned short i = 0; i < height; i++)
	//	{
	//		for (unsigned int j = 0; j < rowbytes; j += 4)
	//		{
	//			*tmp++ = CC_RGB_PREMULTIPLY_ALPHA(row_pointers[i][j], row_pointers[i][j + 1],
	//				row_pointers[i][j + 2], row_pointers[i][j + 3]);
	//		}
	//	}
	//
	//	m_bPreMulti = true;
	//}
	free(row_pointers);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}


const char* VERT_SHADER_TEST = R"(

attribute vec2 aXYZW;
attribute vec2 aUV;
varying vec2 vUV;
void main() {
	gl_Position.xy = aXYZW;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	vUV = aUV;
}

)";

const char* FRAG_SHADER_TEST = R"(

precision mediump float;
varying vec2 vUV;
uniform sampler2D uTex;

void main() {
	gl_FragColor = texture2D( uTex, vUV );
	//gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	//gl_FragColor.r = vUV.t;
	//gl_FragColor.a = 1.0;
}

)";

int main(int argc, char** args)
{
	const int SCREEN_WIDTH = 550;
	const int SCREEN_HEIGHT = 400;

	// Init Window
	SDL_Window* gWindow = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	gWindow = SDL_CreateWindow("MyGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(gWindow);
	SDL_GL_SetSwapInterval(1);

	glewInit();

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_SCISSOR_TEST);

	// Init Texture
	char* filePath = "data/jimmyJump_pack/PNG/CharacterLeft_Walk2.png";

	////This is the output data from decodePNG, which is the pixel data for our texture
	//std::vector<unsigned char> out;
	//unsigned long width, height;
	//_getBytesPNG(filePath, out, width, height);

	SDL_Surface* img = IMG_Load(filePath);

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//
	//glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Init Shader
	GLuint programID = glCreateProgram();

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &VERT_SHADER_TEST, NULL);
	glCompileShader(vertexShaderID);

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);	
	glShaderSource(fragmentShaderID, 1, &FRAG_SHADER_TEST, NULL);
	glCompileShader(fragmentShaderID);

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	GLint success = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, (int*)&success);
	if (success == GL_FALSE)
	{
		GLint maxLen = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLen);

		std::vector<char> infoLog(maxLen);
		glGetProgramInfoLog(programID, maxLen, &maxLen, &infoLog[0]);

		std::printf("%s\n", &infoLog[0]);
		fatalError("link error");
	}

	GLuint uTex = glGetUniformLocation(programID, "uTex");
	GLuint aXY = glGetAttribLocation(programID, "aXYZW");
	GLuint aUV = glGetAttribLocation(programID, "aUV");

	struct Position{
		float x, y;
	};
	struct Vertex{
		Position pos;
		Position uvPos;
	};

	
	while (true)
	{
		// Draw
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glActiveTexture(GL_TEXTURE0);
		
		glUseProgram(programID);
		glEnableVertexAttribArray(aXY);
		glEnableVertexAttribArray(aUV);
	
		glBindTexture(GL_TEXTURE_2D, id);
		glUniform1i(uTex, 0);
		
		Vertex vertices[4];
		vertices[0].pos = { 0, 0 };
		vertices[0].uvPos = { 0, 0 };
		
		vertices[1].pos = { 1, 0 };
		vertices[1].uvPos = { 1, 0 };
		
		vertices[2].pos = { 1, 1 };
		vertices[2].uvPos = { 1, 1 };
		
		vertices[3].pos = { 0, 1 };
		vertices[3].uvPos = { 0, 1 };
		
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)&(vertices[0].pos));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)&(vertices[0].uvPos));
		
		short indices[6] = { 3, 0, 1, 3, 1, 2 };
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)indices);

		//GLfloat vertices[] = {
		//	0.5f, 0.5f, 0.0f,
		//	0.5f, -0.5f, 0.0f,
		//	-0.5f, -0.5f, 0.0f,
		//	-0.5f, 0.5f, 0.0f,
		//	0.0f, 0.0f, -0.707f,
		//};
		//
		//GLubyte indices[] = {
		//	0, 1, 1, 2, 2, 3, 3, 0,
		//	4, 0, 4, 1, 4, 2, 4, 3
		//};
		//
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
		//glEnableVertexAttribArray(0);
		//
		//// Draw lines
		////
		//glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_BYTE, indices);

		//GLuint ret = glGetError();
		//printf("%d\n", ret);
		SDL_GL_SwapWindow(gWindow);
		SDL_Delay(100);
	}
	
	SDL_Quit();

	return 0;
}