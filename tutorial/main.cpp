#include "game.h"

int main(int argc, char *argv[])
{
	Game* inst = Game::inst();
	inst->init();
	inst->run();
	inst->exit();

	return 0;
}