#include "wndtitledmessage.h"
#include "pixeldungeon.h"
#include "highlightedtext.h"
#include "icontitle.h"

WndTitledMessage::WndTitledMessage(Image* icon, const std::string& title, const std::string& message)
:WndTitledMessage(new IconTitle(icon, title), message)
{

}

WndTitledMessage::WndTitledMessage(Component* titlebar, const std::string& message)
:Window()
{
	int width = PixelDungeon::landscape() ? WIDTH_L : WIDTH_P;

	titlebar->setRect(0, 0, width, 0);
	add(titlebar);

	HighlightedText* text = new HighlightedText(6);
	text->text(message, width);
	text->setPos(titlebar->left(), titlebar->bottom() + GAP);
	add(text);

	resize(width, (int)text->bottom());
}