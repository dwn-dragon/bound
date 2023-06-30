#include <SDL3/SDL.h>

#include "app.h"

#include <iostream>
#include <chrono>
#include <thread>

#define SGN_LEFT	-
#define SGN_RIGHT	+

#define SGN_JUMP	-
#define SGN_FALL	+

#define NOW std::chrono::steady_clock::now()

union vec2
{
	struct 
	{
		float x, y;
	};
	struct 
	{
		float w, h;
	};
};

union Entity
{
	SDL_FRect rect;
	struct
	{
		vec2 pos;
		vec2 dim;
	};
};

inline vec2& operator+=(vec2& r, const vec2& l) noexcept {
	r.x += l.x, r.y += l.y;
	return r;
}
inline vec2& operator-=(vec2& r, const vec2& l) noexcept {
	r.x -= l.x, r.y -= l.y;
	return r;
}

Entity	platform = { 0, 432, 640, 8 };

Entity	player	= { 320, 240, 16, 16 };
vec2	speed = { 0, 0 }, accel = { 0, 0 };

//	lateral movement
bool left = 0, right = 0;
float ACCEL = 12500;
vec2 K{ 25, 0.25 };

//	falling
float GRAV = 5000;

//	jumping
bool jump = 0, onGround = 0;
float JUMPS = 1, jumps = 0, JUMP = 1000;

//	speed meter
float tm = 0;
bool started = 0, ended = 0;

void keyUp(SDL_KeyboardEvent& event);
void keyDown(SDL_KeyboardEvent& event);

void update(float dt);

int main(int argc, char const *argv[]) {
	init();

	auto last = NOW;
	float dt = 0, target = 1.0f / 100;

	bool run = true;
	while (run) {
		auto now = NOW;
		dt += std::chrono::duration<float>{ now - last }.count();
		last = now;

		//	input events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
				case SDL_EVENT_QUIT:
					run = false;
					continue;

				case SDL_EVENT_KEY_UP:
					keyUp(event.key);
					break;
				case SDL_EVENT_KEY_DOWN:
					keyDown(event.key);
					break;
			}
		}

		if (dt >= target) {
			dt -= target;
			update(target);
		}
	}

	return 0;
}

void update(float dt) {
	//	speed variance
	accel.x = 0;
	if (right)
		accel.x += SGN_RIGHT ACCEL;
	if (left)
		accel.x += SGN_LEFT ACCEL;
		
	if (jump) {
		if (jumps > 0) {
			--jumps;
			speed.y += SGN_JUMP JUMP;
		}
	}

	vec2 esp = { std::exp(-K.x * dt), std::exp(-K.y * dt) };
	vec2 lim = { accel.x / K.x, accel.y / K.y };
	vec2 vlim = { speed.x - lim.x, speed.y - lim.y };

	auto dx = lim.x * dt + (1 / K.x) * vlim.x * (1 - esp.x);
	auto dy = lim.y * dt + (1 / K.y) * vlim.y * (1 - esp.y);

	player.pos += { dx, dy };

	speed.x = lim.x + vlim.x * esp.x;
	speed.y = lim.y + vlim.y * esp.y;

	//	collision with platform
	if (player.pos.y + player.dim.h >= platform.pos.y) {
		speed.y = 0, accel.y = 0;
		player.pos.y = platform.pos.y - player.dim.h;

		onGround = true;
		jumps = JUMPS;
	}
	else {
		onGround = false;
		accel.y = SGN_FALL GRAV;
	}

	//	loops player
	if (player.pos.x > 640)
		player.pos.x = 0 - player.dim.w;
	if (player.pos.x + player.dim.w < 0)
		player.pos.x = 640;

	if (player.pos.y < 0)
		player.pos.y = 240;

	//	renders
	SDL_SetRenderDrawColor(App.renderer, 22, 23, 29, 255);
	SDL_RenderClear(App.renderer);

	SDL_SetRenderDrawColor(App.renderer, 22, 200, 29, 255);
	SDL_RenderFillRect(App.renderer, &platform.rect);

	SDL_SetRenderDrawColor(App.renderer, 200, 23, 29, 255);
	SDL_RenderFillRect(App.renderer, &player.rect);

	SDL_RenderPresent(App.renderer);
}

void keyUp(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_SCANCODE_SPACE) {
		jump = 0;
	}

	if (event.keysym.scancode == SDL_SCANCODE_A) {
		left = 0;
	}
	if (event.keysym.scancode == SDL_SCANCODE_D) {
		right = 0;
	}
}
void keyDown(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_SCANCODE_SPACE) {
		jump = 1;
	}

	if (event.keysym.scancode == SDL_SCANCODE_A) {
		left = 1;
	}
	if (event.keysym.scancode == SDL_SCANCODE_D) {
		right = 1;
	}
}
