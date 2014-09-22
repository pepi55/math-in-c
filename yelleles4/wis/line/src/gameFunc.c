#include <stdio.h>
#include <math.h>

#include "sdlFunc.h"
#include "gameFunc.h"
#include "../art/bmp.xpm"

void gameLoop (void) {
	char **imgFromSrc;
	//const Uint8 *kbState = SDL_GetKeyboardState(NULL);

	int i, j;
	int backgroundWidth, backgroundHeight;

	int pointWidth, pointHeight;
	float a, b, c, y, a2, b2, c2, y2; //xdiff, ydiff;
	float P_x = 300, P_y = 400, Q_x = 600, Q_y = 100,
				P2_x = 300, P2_y = 500, //Q2_x = 100, Q2_y = 50,
				S_x = 100, S_y = 100;

	SDL_Event e;
	SDL_Texture *background;
	SDL_Texture *points;

	quit = 0;

	imgFromSrc = bmp_xpm;
	background = loadHeader(imgFromSrc, ren);
	points = loadTexture("art/point.png", ren);
	if (background == NULL || points == NULL) {
		logSDLError("loadTexture");
		return;
	}

	SDL_QueryTexture(background, NULL, NULL, &backgroundWidth, &backgroundHeight);
	SDL_QueryTexture(points, NULL, NULL, &pointWidth, &pointHeight);

	backgroundWidth /= 2;
	backgroundHeight /= 2;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = 1;

			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == 1) {
					Q_x = e.motion.x; Q_y = e.motion.y;
				} else if (e.button.button == 3) {
					P_x = e.motion.x; P_y = e.motion.y;
				}
			}
		}

		SDL_RenderClear(ren);

		for (i = 0; i < WINDOW_WIDTH / backgroundWidth; i++) {
			for (j = 0; j <= WINDOW_HEIGHT / backgroundHeight; j++) {
				renderTextureS(background, ren, i * backgroundWidth, j * backgroundHeight, backgroundWidth, backgroundHeight);
			}
		}

		c = 10; a = -1; b = 5;

		for (i = 0; i <= WINDOW_WIDTH; i++) {
			//y = (c - a * i) / b;
			a = -(Q_y - P_y);
			b = Q_x - P_x;
			c = a * P_x + b * P_y;

			y = (c - a * i) / b;

			if (y > 0 && y < WINDOW_WIDTH) {
				SDL_RenderDrawPoint(ren, i, y);
			}

			a2 = -b;
			b2 = a;
			c2 = a2 * S_x + b2 * S_y;

			y2 = (c2 - a2 * i) / b2;

			if (y2 > 0 && y2 < WINDOW_WIDTH) {
				SDL_RenderDrawPoint(ren, i, y2);
			}
		}

		P2_x = (b2 * c2 - b2 * c2) / (a2 * b2 - a2 * b2);
		P2_y = (c2 * a2 - c2 * a2) / (a2 * b2 - a2 * b2);

		renderTexture(points, ren, S_x - pointWidth / 2, S_y - pointHeight / 2);
		//renderTexture(points, ren, Q_x - pointWidth / 2, Q_y - pointHeight / 2);
		renderTexture(points, ren, P2_x - pointWidth / 2, P2_y - pointHeight / 2);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(background);
	SDL_DestroyTexture(points);
}
