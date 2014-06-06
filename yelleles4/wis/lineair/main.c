#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "src/sdlFunc.h"
#include "art/bmp.xpm"

int main (void) {
	//LINE
	int lineWidth, lineHeight, pointWidth, pointHeight;
	int a, b, P_x, P_y;
	int mousePos[2] = {0, 0}, target[2] = {0, 0}, target2[2] = {0, 0};

	P_x = WINDOW_WIDTH / 2; P_y = WINDOW_HEIGHT / 2;
	//LINE

	int WidthBack, HeightBack, i, j;
	bool quit = 0;
	//const Uint8 *kbState = SDL_GetKeyboardState(NULL);
	char **imgFromSrc;

	SDL_Event e;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Texture *background;

	SDL_Texture *line;
	SDL_Texture *points;

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
	points = loadTexture("art/point.png", ren);
	if (background == NULL || line == NULL || points == NULL) {
		logSDLError("loadTexture");
		return 4;
	}

	SDL_RenderClear(ren);

	SDL_QueryTexture(background, NULL, NULL, &WidthBack, &HeightBack);
	SDL_QueryTexture(line, NULL, NULL, &lineWidth, &lineHeight);
	SDL_QueryTexture(points, NULL, NULL, &pointWidth, &pointHeight);

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

			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == 1) {
					mousePos[0] = e.motion.x; mousePos[1] = e.motion.y;
				} else if (e.button.button == 3) {
					P_x = e.motion.x; P_y = e.motion.y;
				}
			}
		}

		SDL_RenderClear(ren);

		for (i = 0; i < WINDOW_WIDTH / WidthBack; i++) {
			for (j = 0; j <= WINDOW_HEIGHT / HeightBack; j++) {
				renderTextureS(background, ren, i * WidthBack, j * HeightBack, WidthBack, HeightBack);
			}
		}

		a = (mousePos[1] - P_y) / (mousePos[0] - P_x);
		b = P_y - a * P_x;

		target[0] = 0; target[1] = a * 0 + b;
		target2[0] = WINDOW_WIDTH; target2[1] = a * WINDOW_WIDTH + b;

		SDL_RenderDrawLine(ren, target[0], target[1], target2[0], target2[1]);

		renderTexture(points, ren, mousePos[0] - pointWidth / 2, mousePos[1] - pointHeight / 2);
		renderTexture(points, ren, P_x - pointWidth / 2, P_y - pointHeight / 2);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(background);
	SDL_DestroyTexture(line);
	SDL_DestroyTexture(points);

	cleanUp(win, ren);

	return 0;
}
