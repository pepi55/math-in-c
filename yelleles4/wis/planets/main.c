#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "src/sdlFunc.h"
#include "art/bmp.xpm"

int main (void) {
	int i, j, x, y, sx, sy, mx, my;
	int WidthBack, HeightBack, WidthSun, HeightSun, WidthMoon, HeightMoon, WidthPlanet, HeightPlanet;

	float a = 0, r = 200;
	float ma = 0, mr = 50;

	bool quit = 0;
	//const Uint8 *kbState = SDL_GetKeyboardState(NULL);
	char **imgFromSrc;

	SDL_Event e;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Texture *background;
	SDL_Texture *sun;
	SDL_Texture *planet;
	SDL_Texture *moon;

	x = 10;
	y = 10;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 || (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError("Init");
		return 1;
	}

	win = SDL_CreateWindow(
			"Planets",
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
	sun = loadTexture("art/point.png", ren);
	planet = loadTexture("art/point.png", ren);
	moon = loadTexture("art/point.png", ren);
	if (background == NULL || sun == NULL || moon == NULL || planet == NULL) {
		logSDLError("loadTexture");
		return 4;
	}

	SDL_RenderClear(ren);

	SDL_QueryTexture(background, NULL, NULL, &WidthBack, &HeightBack);
	SDL_QueryTexture(sun, NULL, NULL, &WidthSun, &HeightSun);
	SDL_QueryTexture(moon, NULL, NULL, &WidthMoon, &HeightMoon);
	SDL_QueryTexture(planet, NULL, NULL, &WidthPlanet, &HeightPlanet);

	WidthSun *= 2;
	HeightSun *= 2;

	WidthMoon /= 2;
	HeightMoon /= 2;

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

		sx = WINDOW_WIDTH / 2;
		sy = WINDOW_HEIGHT / 2;

		x = (int)sx + r * cosf(a);
		y = (int)sy + 1 * r * sinf(a);

		mx = (int)x + mr * cosf(ma);
		my = (int)y + 0.9 * mr * sinf(ma);

		a += 0.01;
		ma -= 0.05;

		renderTextureS(sun, ren, sx - WidthSun / 2, sy - HeightSun / 2, WidthSun, HeightSun);
		renderTextureS(planet, ren, x - WidthPlanet / 2, y - WidthPlanet / 2, WidthPlanet, HeightPlanet);
		renderTextureS(moon, ren, mx - WidthMoon / 2, my - WidthMoon / 2, WidthMoon, HeightMoon);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(background);
	SDL_DestroyTexture(planet);
	SDL_DestroyTexture(moon);
	SDL_DestroyTexture(sun);

	cleanUp(win, ren);

	return 0;
}
