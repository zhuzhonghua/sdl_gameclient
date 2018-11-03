#pragma once

#include "bitmaptext.h"
#include "regex/pcrecpp.h"
#include "rect.h"

namespace Zengine{
class Texture;
class BitmapTextMultiline :public BitmapText{
public:
	class LineSplitter{
	private:
		BitmapTextMultiline* btml;

		std::vector<BitmapText*> lines;

		std::stringstream curLine;
		float curLineWidth;

		Point metrics;

		void newLine(const std::string& str, float width);
		void append(const std::string& str, float width);

	public:
		LineSplitter(BitmapTextMultiline* b) :btml(b){}
		void split(std::vector<BitmapText*>& lines);
	};
public:
	int maxWidth;
	int nLines;
	std::vector<bool> mask;

	BitmapTextMultiline();
	BitmapTextMultiline(const std::string& text);

	struct Word{
		Texture* tex;
		Rect bound;
	};
	static std::map<Uint16, Word> fonts;

	struct SingleWord{
		Texture* tex;
		std::vector<GLSLProgram::Vertex> vertices;

		SingleWord(Texture* t) :tex(t){}
	};
	std::vector<SingleWord>	wordText;

	virtual void draw();
	virtual void measure();

	Word& getWord(Uint16 c);
protected:
	float spaceSize;

	static const pcrecpp::RE PARAGRAPH;
	//static const boost::regex WORD;

	virtual void updateVertices();

private:
	void getWordMetrics(const std::string& word, Point& metrics);

	class SymbolWriter{
	public:
		BitmapTextMultiline* con;

		float width;
		float height;

		int nLines;

		float lineWidth;
		float lineHeight;

		float x;
		float y;

		SymbolWriter(BitmapTextMultiline* con);

		void addSymbol(float w, float h);
		void addSpace(float w);
		void newLine(float w, float h);
		int NLines();
	};
};

}