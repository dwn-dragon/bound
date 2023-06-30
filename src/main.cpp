#include <SDL3/SDL.h>

#include "app.h"

#include <iostream>
#include <chrono>
#include <thread>

#define SGN_LEFT	-
#define SGN_RIGHT	+

#define SGN_JUMP	-
#define SGN_FALL	+

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
vec2	speed = { 0, 0 };

//	lateral movement
bool left = 0, right = 0;
float ACCEL = 8, DECEL = 0.5;
float MAX_MOV_SPEED = 8;

//	falling
float GRAV = 1;
float MAX_FALL_SPEED = 10;

//	jumping
bool jump = 0, onGround = 0;
float JUMPS = 1, jumps = 0, JUMP = 16;

void keyUp(SDL_KeyboardEvent& event);
void keyDown(SDL_KeyboardEvent& event);

int main(int argc, char const *argv[]) {
	init();

	bool run = true;
	while (run) {
		std::this_thread::sleep_for( std::chrono::milliseconds(10) );

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

		//	speed variance
		//	x
		if (right || left) {
			if (right) {
				//	
				auto mv = MAX_MOV_SPEED - SGN_RIGHT speed.x;
				if (mv < 0)
					mv = 0;

				auto acc = (ACCEL < mv) ? ACCEL : mv;
				speed.x += SGN_RIGHT acc;
			}

			if (left) {
				auto mv = MAX_MOV_SPEED - SGN_LEFT speed.x;
				if (mv < 0)
					mv = 0;

				auto acc = (ACCEL < mv) ? ACCEL : mv;
				speed.x += SGN_LEFT acc;
			}
		}
		else {
			float vlen = abs(speed.x);
			vlen -= DECEL;
			if (vlen < 0)
				vlen = 0;
			speed.x = (speed.x < 0 ? -1 : 1) * vlen;
		}
		//	y
		//	jump
		if (jump) {
			if (jumps > 0) {
				--jumps;
				speed.y += SGN_JUMP JUMP;
			}
		}
		//	gravity
		{
			auto mv = MAX_FALL_SPEED - SGN_FALL speed.y;
			if (mv < 0)
				mv = 0;

			auto acc = (GRAV < mv) ? GRAV : mv;
			speed.y += SGN_FALL acc;
		}

		//	moves player
		player.pos += speed;

		//	collision with platform
		if (player.pos.y + player.dim.h >= platform.pos.y) {
			speed.y = 0;
			player.pos.y = platform.pos.y - player.dim.h;

			onGround = true;
			jumps = JUMPS;
		}
		else {
			onGround = false;
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

	return 0;
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
