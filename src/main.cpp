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
float ACCEL = 2500, DECEL = 3000;
float MAX_MOV_SPEED = 500;

//	falling
float GRAV = 5000;
float MAX_FALL_SPEED = 20000;

//	jumping
bool jump = 0, onGround = 0;
float JUMPS = 1, jumps = 0, JUMP = 1000;

void update(float dt);

void keyUp(SDL_KeyboardEvent& event);
void keyDown(SDL_KeyboardEvent& event);

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
	//	x
	if (right || left) {
		if (right) {
			//	
			speed.x += SGN_RIGHT ACCEL * dt;
			if (speed.x > MAX_MOV_SPEED)
				speed.x = MAX_MOV_SPEED;
		}
		if (left) {
			speed.x += SGN_LEFT ACCEL * dt;
			if (speed.x < -MAX_MOV_SPEED)
				speed.x = -MAX_MOV_SPEED;
		}
	}
	else if (speed.x != 0) {
		float vlen = (speed.x < 0) ? -speed.x : speed.x;
		speed.x /= vlen;

		vlen -= DECEL * dt;
		if (vlen < 0)
			vlen = 0;

		speed.x *= vlen;
	}

	//	y
	if (jump) {
		if (jumps > 0 && onGround) {
			--jumps;
			speed.y += SGN_JUMP JUMP;
		}
	}

	{
		speed.y += accel.y * dt;
		if (speed.y > MAX_FALL_SPEED)
			speed.x = MAX_FALL_SPEED;
	}

	//	motions
	player.pos.x += speed.x * dt;
	player.pos.y += speed.y * dt;

	//	collision with platform
	if (player.pos.y + player.dim.h >= platform.pos.y) {
		speed.y = 0;
		player.pos.y = platform.pos.y - player.dim.h;

		onGround = true;
		jumps = JUMPS;
		accel.y = 0;
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
