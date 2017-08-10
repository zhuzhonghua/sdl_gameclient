#pragma once

#include "bitmaptext.h"
#include <string>
#include <vector>
#include <map>

class BitmapTextMultiline :public BitmapText{
public:
	int maxWidth;
	int nLines;
	std::vector<bool> mask;

	BitmapTextMultiline();
	BitmapTextMultiline(const std::string& text);

	struct Word{
		SmartTexture* tex;
		RectF bound;
	};
	static std::map<Uint16, Word> fonts;

	struct SingleWord{
		SmartTexture* tex;
		std::vector<float> vertices;

		SingleWord(SmartTexture* t) :tex(t){}
	};
	std::vector<SingleWord>	wordText;

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