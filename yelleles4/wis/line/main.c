#include <stdio.h>
#include <stdbool.h>

#include "src/sdlFunc.h"
#include "src/gameFunc.h"

//Compile with:
//gcc -ggdb -Wall -lm -lSDL2 -lSDL2_image -o test main.c src/sdlFunc.c src/gameFunc.c

int main (void) {
	initGL_SDL();

	gameLoop();

	cleanUp(win, glwin, ren);

	return 0;
}
