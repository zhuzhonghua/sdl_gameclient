#include "stdafx.h"
#include "bitmaptextmultiline.h"

#include "glslprogram.h"

namespace Zengine{
const pcrecpp::RE BitmapTextMultiline::PARAGRAPH("(.*)\n", pcrecpp::UTF8());
//const boost::regex BitmapTextMultiline::WORD("\\s+");

void BitmapTextMultiline::LineSplitter::newLine(const std::string& str, float width)
{
	BitmapText* txt = new BitmapText(curLine.str());

	//txt.scale.set(scale.x);
	lines.push_back(txt);

	curLine = std::stringstream();
	curLine << str;
	curLineWidth = width;
}

void BitmapTextMultiline::LineSplitter::append(const std::string& str, float width)
{
	curLineWidth += (curLineWidth > 0 ? btml->tracking() : 0) + width;
	curLine << str;
}

void BitmapTextMultiline::LineSplitter::split(std::vector<BitmapText*>& lines)
{
	curLine = std::stringstream();
	curLineWidth = 0;

	std::vector<std::string> paragraphs;

	std::string txt = btml->text();
	pcrecpp::StringPiece text(txt);
	int counter = 0;
	std::string line;
	while (PARAGRAPH.Consume(&text, &line))
	{
		paragraphs.push_back(line);
	}
	paragraphs.push_back(text.as_string());

	for (int i = 0; i < paragraphs.size(); i++)
	{
		std::vector<std::string> words;
		Util::splitUTF8(paragraphs[i], words);

		for (int j = 0; j < words.size(); j++)
		{
			int w = 0, h = 0;
			TTF_SizeUTF8(_font, words[j].c_str(), &w, &h);

			metrics = Point(w, h);

			if (curLineWidth > 0 && curLineWidth + btml->tracking() + metrics.x > btml->maxWidth / 1)
			{
				newLine(words[j], metrics.x);
			}
			else
			{
				append(words[j], metrics.x);
			}

			if (curLineWidth > 0 && curLineWidth + btml->tracking() + btml->spaceSize > btml->maxWidth / 1)
			{
				newLine("", 0);
			}
			else
			{
				//append(" ", btml->spaceSize);
			}
		}

		newLine("", 0);
	}

	lines.assign(this->lines.begin(), this->lines.end());
}

std::map<Uint16, BitmapTextMultiline::Word> BitmapTextMultiline::fonts;

BitmapTextMultiline::BitmapTextMultiline()
	:BitmapTextMultiline("")
{

}

BitmapTextMultiline::BitmapTextMultiline(const std::string& text)
	: BitmapText(text)
{
	maxWidth = INT_MAX_VALUE;
	nLines = 0;
	spaceSize = 1;
}

void BitmapTextMultiline::getWordMetrics(const std::string& word, Point& metrics)
{
	int w, h;
	TTF_SizeUTF8(_font, word.c_str(), &w, &h);

	metrics = Point(w,h);
}

void BitmapTextMultiline::updateVertices()
{
	if (_text.size() <= 0) return;

	std::vector<std::string> v;

	pcrecpp::StringPiece text(_text);
	int counter = 0;
	std::string line;
	while (PARAGRAPH.Consume(&text, &line))
	{
		v.push_back(line);
	}
	v.push_back(text.as_string());

	float vx = 0, vy = 0;
	SymbolWriter writer(this);

	for (int i = 0; i < v.size(); i++)
	{
		const SDL_Color c = { 255, 255, 255 };

		std::vector<Uint16> words;
		Util::splitUTF8String(v[i], words);
		for (int j = 0; j < words.size(); j++)
		{
			const Word& w = getWord(words[j]);
			writer.addSymbol(w.bound.width(), w.bound.height());

			wordText.emplace_back(w.tex);
			SingleWord& vw = wordText.back();
			vw.vertices.resize(16);

			float wf = w.bound.width();
			float hf = w.bound.height();

			float left = 0;
			float top = 0;
			float right = 1;
			float bottom = 1;

			_vertices[0].setPos(writer.x + wf, writer.y + hf);
			_vertices[0].setUV(1, 0);

			_vertices[1].setPos(writer.x, writer.y + hf);
			_vertices[1].setUV(0, 0);

			_vertices[2].setPos(writer.x, writer.y);
			_vertices[2].setUV(0, 1);

			_vertices[3].setPos(writer.x + wf, writer.y);
			_vertices[3].setUV(1, 1);
		}

		writer.newLine(0, lineHeight());
	}
}

void BitmapTextMultiline::draw()
{
	BitmapText::draw();

	GLSLProgram* script = GLSLProgram::inst();

	if (_dirty)
	{
		updateVertices();
		_dirty = false;
	}

	for (int i = 0; i < wordText.size(); i++)
	{
		wordText[i].tex->bind();

		script->drawQuad(&wordText[i].vertices[0]);
	}
}

BitmapTextMultiline::Word& BitmapTextMultiline::getWord(Uint16 c)
{
	const SDL_Color color = { 255, 255, 255 };

	// TODO: profile, multi word on to one texture
	std::map<Uint16, BitmapTextMultiline::Word>::iterator itr = fonts.find(c);
	if (itr == fonts.end())
	{
		SDL_Surface* surface = TTF_RenderGlyph_Blended(_font, c, color);
		Word w;
		w.tex = new Texture(surface);
		w.bound = Rect(0, 0, surface->w, surface->h);
		fonts.insert(std::make_pair(c, w));
		SDL_FreeSurface(surface);

		itr = fonts.find(c);
	}

	return itr->second;
}

void BitmapTextMultiline::measure()
{
	const SDL_Color c = { 255, 255, 255 };

	std::vector<std::string> v;

	pcrecpp::StringPiece text(_text);
	int counter = 0;
	std::string line;
	while (PARAGRAPH.Consume(&text, &line))
	{
		v.push_back(line);
	}
	v.push_back(text.as_string());

	SymbolWriter writer(this);

	for (int i = 0; i < v.size(); i++)
	{
		std::vector<Uint16> words;
		Util::splitUTF8String(v[i], words);
		for (int j = 0; j < words.size(); j++)
		{
			if (j > 0)
			{
				writer.addSpace(spaceSize);
			}

			const Word& w = getWord(words[j]);
			writer.addSymbol(w.bound.width(), w.bound.height());
		}

		writer.newLine(0, lineHeight());
	}

	_width = writer.width;
	_height = writer.height;

	nLines = writer.NLines();
}

BitmapTextMultiline::SymbolWriter::SymbolWriter(BitmapTextMultiline* con)
{
	this->con = con;

	width = 0;
	height = 0;

	nLines = 0;

	lineWidth = 0;
	lineHeight = 0;

	x = 0;
	y = 0;
}

int BitmapTextMultiline::SymbolWriter::NLines()
{
	return x == 0 ? nLines : nLines + 1;
}

void BitmapTextMultiline::SymbolWriter::newLine(float w, float h)
{
	height += lineHeight;
	if (width < lineWidth)
	{
		width = lineWidth;
	}

	lineWidth = w;
	lineHeight = h;

	x = 0;
	y = height;

	nLines++;
}

void BitmapTextMultiline::SymbolWriter::addSpace(float w)
{
	if (lineWidth > 0 && lineWidth + con->tracking() + w > con->maxWidth / 1)
	{
		newLine(0, 0);
	}
	else
	{
		x = lineWidth;
		lineWidth += (lineWidth > 0 ? con->tracking() : 0) + w;
	}
}

void BitmapTextMultiline::SymbolWriter::addSymbol(float w, float h)
{
	if (lineWidth > 0 && lineWidth + con->tracking() + w > con->maxWidth / 1)
	{
		newLine(w, h);
	}
	else
	{
		x = lineWidth;

		lineWidth += (lineWidth > 0 ? con->tracking() : 0) + w;
		if (h > lineHeight)
		{
			lineHeight = h;
		}
	}
}
}