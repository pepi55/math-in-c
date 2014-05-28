#pragma once

#define SDL_FUNC_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define M_PI 3.14159265358979323846

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 460
#define FRAMES_PER_SECOND 60
#define FRAMERATE = 1000 / FRAMES_PER_SECOND

#define WINDOW_OFFSET_X 200
#define WINDOW_OFFSET_Y 180

void logSDLError(char *err);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTextureS(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void renderTextureR(SDL_Texture *tex, SDL_Renderer *ren, int *target, int x, int y);
void renderSprite(SDL_Texture *tex, SDL_Renderer *ren, int y, int x, SDL_Rect *clip);
void cleanUp(SDL_Window *win, SDL_Renderer *ren);

SDL_Texture *loadBmp(char *loc, SDL_Renderer *ren);
SDL_Texture *loadTexture(char *loc, SDL_Renderer *ren);
SDL_Texture *loadHeader(char **loc, SDL_Renderer *ren);
