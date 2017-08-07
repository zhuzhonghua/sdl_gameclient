#include "bitmaptextmultiline.h"
#include "define.h"
#include "util.h"
#include "noosascript.h"

//const boost::regex BitmapTextMultiline::PARAGRAPH("\n");
//const boost::regex BitmapTextMultiline::WORD("\\s+");

BitmapTextMultiline::BitmapTextMultiline()
:BitmapTextMultiline("")
{

}

BitmapTextMultiline::BitmapTextMultiline(const std::string& text)
: BitmapText(text)
{
	maxWidth = INT_MAX_VALUE;
	nLines = 0;
}

void BitmapTextMultiline::getWordMetrics(const std::string& word, PointF& metrics)
{
	int w, h;
	TTF_SizeUTF8(_font, word.c_str(), &w, &h);

	GameMath::PointFSet(&metrics, w, h);
}

void BitmapTextMultiline::updateVertices()
{
	if (_text.size() <= 0) return;

	for (int i = 0; i < lines.size(); i++)
	{
		delete lines[i].texture;
	}
	lines.clear();

	std::vector<std::string> v;
	GameMath::splitString(_text, v, "\n");
	
	for (int i = 0; i < v.size(); i++)
	{
		lines.emplace_back(v[i]);
	}

	float vx = 0, vy = 0;
	SymbolWriter writer(this);

	for (int i = 0; i < lines.size(); i++)
	{
		const SDL_Color c = { 255, 255, 255 };

		SDL_Surface* surface = TTF_RenderUTF8_Blended(_font, lines[i].text.c_str(), c);
		lines[i].texture = new SmartTexture(surface);
		lines[i].vertices.resize(16);

		float left = 0;
		float top = 0;
		float right = 1;
		float bottom = 1;

		int wf, hf;
		TTF_SizeUTF8(_font, lines[i].text.c_str(), &wf, &hf);
		writer.addSymbol(wf,hf);

		lines[i].vertices[0] = writer.x;
		lines[i].vertices[1] = writer.y;

		lines[i].vertices[2] = left;
		lines[i].vertices[3] = top;

		lines[i].vertices[4] = writer.x+wf;
		lines[i].vertices[5] = writer.y;

		lines[i].vertices[6] = right;
		lines[i].vertices[7] = top;

		lines[i].vertices[8] = writer.x + wf;
		lines[i].vertices[9] = writer.y + hf;
		lines[i].vertices[10] = right;
		lines[i].vertices[11] = bottom;

		lines[i].vertices[12] = writer.x;
		lines[i].vertices[13] = writer.y + hf;
		lines[i].vertices[14] = left;
		lines[i].vertices[15] = bottom;

		SDL_FreeSurface(surface);

		writer.newLine(0, hf);
	}
}

void BitmapTextMultiline::draw()
{
	BitmapText::draw();

	NoosaScript* script = NoosaScript::get();

	if (_dirty)
	{
		updateVertices();
		_dirty = false;
	}

	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].texture->bind();
		script->camera(camera());
		script->uModel.valueM4(_mat);
		script->lighting(
			rm, gm, bm, am,
			ra, ga, ba, aa);
		script->drawQuad(&lines[i].vertices[0]);
	}
}

void BitmapTextMultiline::measure()
{
	std::vector<std::string> v;
	GameMath::splitString(_text, v, "\n");

	SymbolWriter writer(this);

	for (int i = 0; i < v.size(); i++)
	{
		std::string line = v[i];

		int wf, hf;
		TTF_SizeUTF8(_font, line.c_str(), &wf, &hf);
		writer.addSymbol(wf, hf);

		writer.newLine(0, hf);
	}

	widthf = writer.width;
	heightf = writer.height;

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
	if (lineWidth > 0 && lineWidth + con->tracking() + w > con->maxWidth / con->scale.x)
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
	if (lineWidth > 0 && lineWidth + con->tracking() + w > con->maxWidth / con->scale.x) 
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