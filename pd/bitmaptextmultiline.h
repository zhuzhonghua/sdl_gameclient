#pragma once

#include "bitmaptext.h"
//#include "boost/regex.hpp"
#include <string>
#include <vector>

class BitmapTextMultiline :public BitmapText{
public:
	int maxWidth;
	int nLines;
	std::vector<bool> mask;

	BitmapTextMultiline();
	BitmapTextMultiline(const std::string& text);

	struct Line{
		Line(const std::string& t) :text(t){}
		std::string text;
		std::vector<float> vertices;
		SmartTexture* texture;
	};
	std::vector<Line> lines;

	virtual void draw();
	virtual void measure();
protected:
	float spaceSize;

	//static const boost::regex PARAGRAPH;
	//static const boost::regex WORD;

	virtual void updateVertices();

private:
	void getWordMetrics(const std::string& word, PointF& metrics);

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