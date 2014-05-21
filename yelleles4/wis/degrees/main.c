#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "sdlFunc.h"
#include "bmp.xpm"

#define MAX_POINTS 5

int main (void) {
	//DEGREES
	int mousePos[2];
	//DEGREES

	int WidthBack, HeightBack, i, j;
	bool quit = 0;
	//const Uint8 *kbState = SDL_GetKeyboardState(NULL);
	char **imgFromSrc;

	SDL_Event e;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Texture *background;
	SDL_Texture *arrow;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 || (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError("Init");
		return 1;
	}

	win = SDL_CreateWindow(
			"Arrow click n stuffs",
			WINDOW_OFFSET_X,
			WINDOW_OFFSET_Y,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_OPENGL
			| SDL_WINDOW_ALLOW_HIGHDPI
			| SDL_WINDOW_BORDERLESS
			| SDL_WINDOW_INPUT_GRABBED
			);
	if (win == NULL) {
		logSDLError("CreateWindow");
		return 2;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL) {
		logSDLError("CreateRenderer");
		return 3;
	}

	imgFromSrc = bmp_xpm;
	background = loadHeader(imgFromSrc, ren);
	arrow = loadTexture("arrow.png", ren);
	if (background == NULL || arrow == NULL) {
		logSDLError("loadTexture");
		return 4;
	}

	SDL_RenderClear(ren);

	SDL_QueryTexture(background, NULL, NULL, &WidthBack, &HeightBack);

	WidthBack /= 2;
	HeightBack /= 2;

	for (i = 0; i < WINDOW_WIDTH / WidthBack; i++) {
		for (j = 0; j <= WINDOW_HEIGHT / HeightBack; j++) {
			renderTextureS(background, ren, i * WidthBack, j * HeightBack, WidthBack, HeightBack);
		}
	}

	SDL_RenderPresent(ren);

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = 1;

			if (e.type == SDL_MOUSEBUTTONDOWN) { //quit = 1;
				mousePos[0] = e.motion.x; mousePos[1] = e.motion.y;
			}
		}

		SDL_RenderClear(ren);

		for (i = 0; i < WINDOW_WIDTH / WidthBack; i++) {
			for (j = 0; j <= WINDOW_HEIGHT / HeightBack; j++) {
				renderTextureS(background, ren, i * WidthBack, j * HeightBack, WidthBack, HeightBack);
			}
		}

		renderTextureR(arrow, ren, mousePos, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(background);
	SDL_DestroyTexture(arrow);

	cleanUp(win, ren);

	return 0;
}
