#include <stdio.h>
#include <string>

#include "root.h"
#include "movie.h"
//#include "test.h"

const int SCREEN_WIDTH = 550;
const int SCREEN_HEIGHT = 400;
int main(int argc, char** args)
{
	//testLoadIMG();

	Root r;
	r.init(SCREEN_HEIGHT, SCREEN_WIDTH);
	Movie* m = r.createMovie("testpng3.swf");
	m->play();
	r.run();

	return 0;
}