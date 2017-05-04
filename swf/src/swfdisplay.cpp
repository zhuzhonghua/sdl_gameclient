#include "swfdisplay.h"
#include "movie.h"
#include "character.h"

SWFDisplay::SWFDisplay()
:depth(-1),
ch(0),
ratio(-1),
clipDepth(-1)
{

}

void SWFDisplay::update(Movie* m)
{
	ch->update(m, mat, ratio);
}