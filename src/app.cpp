#include "app.h"
#include <iostream>

_app_t App{ nullptr, nullptr };

void init() {
	int width = 640, height = 480;

	int rendererFlags = SDL_RENDERER_ACCELERATED;
	int windowFlags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL Init error: " << SDL_GetError() << std::endl;
		exit(1);
	}

	App.window = SDL_CreateWindow("Bound", width, height, windowFlags);
	if (!App.window) {
		std::cerr << "SDL Window error: " << SDL_GetError() << std::endl;
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	App.renderer = SDL_CreateRenderer(App.window, nullptr, rendererFlags);
	if (!App.renderer) {
		std::cerr << "SDL Renderer error: " << SDL_GetError() << std::endl;
		exit(1);
	}
}
