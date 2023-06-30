#pragma once

#include <SDL3/SDL.h>

struct _app_t
{
	SDL_Window *window;
	SDL_Renderer *renderer;
};

extern _app_t App;

void init();
