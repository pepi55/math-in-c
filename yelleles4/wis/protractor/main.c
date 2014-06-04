#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "src/sdlFunc.h"
#include "art/bmp.xpm"

int main (void) {
	//PROTRACTOR
	int lineHeight, lineWidth;
	int target[2] = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
	int *location = radToDeg(target, 200, 0);
	//PROTRACTOR

	int WidthBack, HeightBack, i, j;
	bool quit = 0;
	//const Uint8 *kbState = SDL_GetKeyboardState(NULL);
	char **imgFromSrc;

	SDL_Event e;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Texture *background;
	SDL_Texture *line;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 || (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError("Init");
		return 1;
	}

	win = SDL_CreateWindow(
			"Generic Title",
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
	line = loadTexture("art/line.png", ren);
	if (background == NULL || line == NULL) {
		logSDLError("loadTexture");
		return 4;
	}

	SDL_RenderClear(ren);

	SDL_QueryTexture(background, NULL, NULL, &WidthBack, &HeightBack);
	SDL_QueryTexture(line, NULL, NULL, &lineWidth, &lineHeight);

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
			if (e.type == SDL_MOUSEBUTTONDOWN) quit = 1;
		}

		SDL_RenderClear(ren);

		for (i = 0; i < WINDOW_WIDTH / WidthBack; i++) {
			for (j = 0; j <= WINDOW_HEIGHT / HeightBack; j++) {
				renderTextureS(background, ren, i * WidthBack, j * HeightBack, WidthBack, HeightBack);
			}
		}

		for (i = 0; i < 360; i++) {
			if (i % 10 == 0) {
				lineWidth /= 2;
				lineHeight /= 2;
			} else {
				lineWidth /= 4;
				lineHeight /= 4;
			}

			location = radToDeg(target, 200, i);
			location[0] -= lineWidth / 2;
			location[1] -= lineHeight / 2;

			renderTextureRS(line, ren, target, location[0], location[1], lineWidth, lineHeight);

			SDL_QueryTexture(line, NULL, NULL, &lineWidth, &lineHeight);
		}

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(background);
	SDL_DestroyTexture(line);

	cleanUp(win, ren);

	return 0;
}
