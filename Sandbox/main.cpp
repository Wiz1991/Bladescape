#include "Game.hpp"

#ifdef __linux
#include <X11/Xlib.h>
#endif // __linux

int main()
{
#ifdef __linux
	XInitThreads();
#endif //__linux

	Game game;
	game.run();

	return 0;
}