#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "sdlFunc.h"
#include "bmp.xpm"

int main (void) {
	//VECTORS
	pnt A;
	pnt B;

	A.x = 150;
	A.y = 100;

	B.x = 500;
	B.y = 380;

	int speed = 10;
	int vectorA[2] = {A.x, A.y}, vectorB[2] = {B.x, B.y}, vectorP[2] = {200, 200}, vectorH[2], vectorR[2];
	//VECTORS

	int WidthBack, HeightBack, i, j;
	bool quit = 0, gotoAndStopA = 1;
	//const Uint8 *kbState = SDL_GetKeyboardState(NULL);
	char **imgFromSrc;

	SDL_Event e;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Texture *background;
	SDL_Texture *points;
	SDL_Texture *character;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 || (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError("Init");
		return 1;
	}

	win = SDL_CreateWindow("Vector Movement", WINDOW_OFFSET_X, WINDOW_OFFSET_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
	points = loadTexture("point.png", ren);
	character = loadTexture("char.png", ren);
	if (background == NULL || points == NULL || character == NULL) {
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

	//WidthPoint = 50;
	//HeightPoint = 50;

	renderTexture(points, ren, A.x, A.y);
	renderTexture(points, ren, B.x, B.y);

	renderTexture(character, ren, vectorP[0], vectorP[1]);

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

		renderTexture(points, ren, vectorA[0], vectorA[1]);
		renderTexture(points, ren, vectorB[0], vectorB[1]);

		if (gotoAndStopA == 1) {
			vectorA[0] = A.x; vectorA[1] = A.y; vectorB[0] = B.x; vectorB[1] = B.y;
			vectorR[0] = vectorA[0] - vectorP[0]; vectorR[1] = vectorA[1] - vectorP[1];
			vectorH[0] = speed * vectorR[0] / vectorLen(vectorR); vectorH[1] = speed * vectorR[1] / vectorLen(vectorR);
			vectorP[0] += vectorH[0]; vectorP[1] += vectorH[1];

			renderTextureR(character, ren, vectorA, vectorP[0], vectorP[1]);

			if (vectorLen(vectorR) <= speed / 2) {
				SDL_Delay(1000);

				gotoAndStopA = 0;
			}
		} else if (gotoAndStopA == 0) {
			vectorB[0] = B.x; vectorB[1] = B.y; vectorA[0] = A.x; vectorA[1] = A.y;
			vectorR[0] = vectorB[0] - vectorP[0]; vectorR[1] = vectorB[1] - vectorP[1];
			vectorH[0] = speed * vectorR[0] / vectorLen(vectorR); vectorH[1] = speed * vectorR[1] / vectorLen(vectorR);
			vectorP[0] += vectorH[0]; vectorP[1] += vectorH[1];

			renderTextureR(character, ren, vectorB, vectorP[0], vectorP[1]);

			if (vectorLen(vectorR) <= speed / 2) {
				SDL_Delay(1000);

				gotoAndStopA = 1;
			}
		}

		SDL_Delay(50);
		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(background);
	SDL_DestroyTexture(points);
	SDL_DestroyTexture(character);

	cleanUp(win, ren);

	return 0;
}

float vectorLen (int *vecR) {
	float result = 0;

	if (&vecR[0] != NULL && &vecR[1] != NULL) {
		float sqo = powf(vecR[0], 2), sqt = powf(vecR[1], 2);
		float sq = sqo + sqt;

		result = sqrtf(sq);
	}

	return result;
}
