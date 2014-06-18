#include <stdio.h>
#include <math.h>
#include <SDL2/SDL_image.h>

#include "sdlFunc.h"

void logSDLError(char *err) {
	fprintf(stderr, "\nError: %s, at %s\n", SDL_GetError(), err);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	SDL_Rect canv;
	SDL_QueryTexture(tex, NULL, NULL, &canv.w, &canv.h);

	canv.x = x;
	canv.y = y;

	SDL_RenderCopy(ren, tex, NULL, &canv);
}

void renderTextureS(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
	SDL_Rect canv;
	SDL_QueryTexture(tex, NULL, NULL, &canv.w, &canv.h);

	canv.x = x;
	canv.y = y;
	canv.w = w;
	canv.h = h;

	SDL_RenderCopy(ren, tex, NULL, &canv);
}

void renderTextureR (SDL_Texture *tex, SDL_Renderer *ren, int *target, int x, int y) {
	float xdiff, ydiff, radians;
	double degrees = 0;

	SDL_Rect canv;
	SDL_QueryTexture(tex, NULL, NULL, &canv.w, &canv.h);

	canv.x = x;
	canv.y = y;

	if (&target[0] != 0 && &target[1] != 0) {
		xdiff = x - target[0];
		ydiff = y - target[1];
		radians = atan2(ydiff, xdiff);
		degrees = (double)(radians * 180 / M_PI);
	}

	SDL_RenderCopyEx(ren, tex, NULL, &canv, degrees, NULL, SDL_FLIP_NONE);
}

void renderTextureRS(SDL_Texture *tex, SDL_Renderer *ren, int *target, int x, int y, int w, int h) {
	float xdiff, ydiff, radians;
	double degrees = 0;

	SDL_Rect canv;
	SDL_QueryTexture(tex, NULL, NULL, &canv.w, &canv.h);

	canv.x = x;
	canv.y = y;
	canv.w = w;
	canv.h = h;

	if (&target[0] != 0 && &target[1] != 0) {
		xdiff = x - target[0];
		ydiff = y - target[1];
		radians = atan2(ydiff, xdiff);
		degrees = (double)(radians * 180 / M_PI);
	}

	SDL_RenderCopyEx(ren, tex, NULL, &canv, degrees, NULL, SDL_FLIP_NONE);
}

void renderSprite(SDL_Texture *tex, SDL_Renderer *ren, int y, int x, SDL_Rect *clip) {
	SDL_Rect dst;

	dst.x = x;
	dst.y = y;

	if (clip != NULL) {
		dst.w = clip->w;
		dst.h = clip->h;
	} else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}

	SDL_RenderCopy(ren, tex, clip, &dst);
}

void initGL_SDL (void) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0 || (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError("Init");
		quit = 1;

		return;
	}

	win = SDL_CreateWindow(
			WINDOW_NAME,
			WINDOW_OFFSET_X,
			WINDOW_OFFSET_Y,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_OPENGL
			//| SDL_WINDOW_FULLSCREEN
			| SDL_WINDOW_RESIZABLE
			| SDL_WINDOW_SHOWN
			//| SDL_WINDOW_ALLOW_HIGHDPI
			//| SDL_WINDOW_BORDERLESS
			//| SDL_WINDOW_INPUT_GRABBED
			);
	if (win == NULL) {
		logSDLError("CreateWindow");
		quit = 1;

		return;
	}

	glwin = SDL_GL_CreateContext(win);
	if (glwin == NULL) {
		logSDLError("GLCreateContext");
		quit = 1;

		return;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL) {
		logSDLError("CreateRenderer");
		quit = 1;

		return;
	}
}

void cleanUp(SDL_Window *win, SDL_GLContext *glwin, SDL_Renderer *ren) {
	SDL_DestroyRenderer(ren);
	SDL_GL_DeleteContext(glwin);
	SDL_DestroyWindow(win);

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture *loadBmp(char *loc, SDL_Renderer *ren) {
	SDL_Texture *tex = NULL;
	SDL_Surface *loadedImg = SDL_LoadBMP(loc);

	if (loadedImg != NULL) {
		tex = SDL_CreateTextureFromSurface(ren, loadedImg);
		SDL_FreeSurface(loadedImg);

		if (tex == NULL) logSDLError("CreateTextureFromSurface");
	} else {
		logSDLError("LoadBMP");
	}

	return tex;
}

SDL_Texture *loadTexture(char *loc, SDL_Renderer *ren) {
	SDL_Texture *tex = IMG_LoadTexture(ren, loc);
	if (tex == NULL) logSDLError("LoadTexture");

	return tex;
}

SDL_Texture *loadHeader(char **loc, SDL_Renderer *ren) {
	SDL_Texture *tex = NULL;
	SDL_Surface *loadedSource = IMG_ReadXPMFromArray(loc);

	if (loadedSource != NULL) {
		tex = SDL_CreateTextureFromSurface(ren, loadedSource);
		SDL_FreeSurface(loadedSource);

		if (tex == NULL) logSDLError("CreateTextureFromSource");
	} else {
		logSDLError("LoadSRC");
	}

	return tex;
}
