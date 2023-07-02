#include <SDL3/SDL.h>
#include <iostream>

#include "linm.h"

#include "app.h"
#include "map.h"
#include "game.h"

#include <chrono>

using _clk = std::chrono::steady_clock;
using _time = std::chrono::duration<float>;

void events();
void close();

void draw();
void update(_time dt);

void mouseMov(SDL_MouseMotionEvent& event);
void mouseUp(SDL_MouseButtonEvent& event);
void mouseDown(SDL_MouseButtonEvent& event);
void keyUp(SDL_KeyboardEvent& event);
void keyDown(SDL_KeyboardEvent& event);

uint8_t clrColor[] = {  22,  23,  29, 255 };
uint8_t mapColor[] = {  22, 200,  29, 255 };
uint8_t plrColor[] = { 200,  23,  29, 255 };

Map 	map;
Entity	player;

bool run = false;

int main(int argc, char const *argv[]) {
	init();

	map = Map{
		Entity{  0,  0,  1, 60 },
		Entity{ 79,  0,  1, 60 },
		Entity{  1,  0, 78,  1 },
		Entity{  1, 59, 78,  1 }
	};

	player = { 20, 15, 1, 1 };

	auto last = _clk::now();
	_time dt{ 0 }, target{ 1.0f / 100 };

	run = true;
	while (run) {
		auto now = _clk::now();
		dt += now - last;
		last = now;

		events();
		
		if (dt >= target) {
			dt -= target;

			update(target);
			draw();
		}
	}

	return 0;
}

void draw() {
	setColor(clrColor);
	clear();

	setColor(mapColor);
	for (auto it = map.begin(), end = map.end(); it != end; it++)
		renderFillRect(*it);

	setColor(plrColor);
	renderFillRect(player);

	present();
}
void update(_time dt) {
}

void keyUp(SDL_KeyboardEvent& event) {
}
void keyDown(SDL_KeyboardEvent& event) {
}

void mouseMov(SDL_MouseMotionEvent& event) {
}
void mouseUp(SDL_MouseButtonEvent& event) {
}
void mouseDown(SDL_MouseButtonEvent& event) {
}

void events() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) switch (event.type)
	{
	case SDL_EVENT_QUIT:
		close();
		break;

	case SDL_EVENT_KEY_UP:
		keyUp(event.key);
		break;
	case SDL_EVENT_KEY_DOWN:
		keyDown(event.key);
		break;
	}
}
void close() {
	run = false;
}
