#include <SDL3/SDL.h>

#include "linm.h"

#include "app.h"
#include "game.h"
#include "map.h"
#include "input.h"

#include <chrono>
#include <iostream>
#include <iomanip>

#define	SGN_RIGHT	+
#define	SGN_LEFT	-

#define	SGN_JUMP	-
#define	SGN_FALL	+

using _clk = std::chrono::steady_clock;
using _time = std::chrono::duration<float>;

void events();
void close();

void draw();
void update(float dt);

void collision(float grav, vec2 ds);
void cWeapon(float grav, vec2 ds);

void movMixed(float dt);
void movWeapon(float dt);

void warp_wpn();
void throw_wpn(float vel, vec2 dir);

void mouseMov(SDL_MouseMotionEvent& event);
void mouseUp(SDL_MouseButtonEvent& event);
void mouseDown(SDL_MouseButtonEvent& event);

void keyUp(SDL_KeyboardEvent& event);
void keyDown(SDL_KeyboardEvent& event);

uint8_t clrColor[] = {  22,  23,  29, 255 };
uint8_t mapColor[] = {  22, 200,  29, 255 };
uint8_t plrColor[] = { 200,  23,  29, 255 };
uint8_t wpnColor[] = { 200,  23, 200, 255 };

struct CollResult
{
	bool collided;
	float ds;
};

enum : uint8_t
{
	NONE = 0,
	UP,
	DOWN,
	RIGHT,
	LEFT
};

bool run = false;
bool left = false, right = false, jump = false;
bool mleft = false, mright = false;

bool onGround = false;
float JUMPS = 1, jumps = 0;

Data data;

Map map;

Entity player;
vec2 speed = { 0, 0 }, accel = { 0, 0 };

Entity weapon;
bool rWpn = false;
Entity* blk = nullptr;
uint8_t side = NONE;
vec2 vWpn = { 0, 0 }, aWpn = { 0, 0 };

int main(int argc, char const *argv[]) {
	init();
	std::cout << std::fixed << std::setprecision(2);

	map = readMap("map.dat");
	
	player = { 20, 15, 1, 1 };
	weapon = { 0, 0, 0.25, 0.25 };

	data = { {
		//	movement
		25, 125, -125,
		//	jumps
		600, -120
	} };

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

			update(target.count());
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

	if (rWpn) {
		setColor(wpnColor);
		renderFillRect(weapon);
	}

	present();
}
void update(float dt) {
	//	
	movMixed(dt);

	//	
	if (rWpn)
		movWeapon(dt);

	//	loops player
	if (player.pos.x > 640)
		player.pos.x = 0 - player.dim.w;
	if (player.pos.x + player.dim.w < 0)
		player.pos.x = 640;
	//	
	if (player.pos.y > 480)
		player.pos.y = 0 - player.dim.h;
	if (player.pos.y + player.dim.h < 0)
		player.pos.y = 480;
}

void movMixed(float dt) {
	//	speed variance
	//	x
	if (right || left) {
		if (right) {
			//	
			speed.x += SGN_RIGHT data.mixed.accel * dt;
			if (speed.x > data.mixed.limit)
				speed.x = data.mixed.limit;
		}
		if (left) {
			speed.x += SGN_LEFT data.mixed.accel * dt;
			if (speed.x < -data.mixed.limit)
				speed.x = -data.mixed.limit;
		}
	}
	else if (speed.x != 0) {
		float vlen = (speed.x < 0) ? -speed.x : speed.x;
		speed.x /= vlen;

		vlen += data.mixed.decel * dt;
		if (vlen < 0)
			vlen = 0;

		speed.x *= vlen;
	}

	//	y
	if (jump) {
		if (jumps > 0 && onGround) {
			--jumps;
			speed.y += data.mixed.vjump;
		}
	}

	speed.y += accel.y * dt;

	auto ds = speed * dt;

	//	collisions
	collision(data.mixed.gravity, ds);
}
void movWeapon(float dt) {
	constexpr float ACCEL = -5;

	//	moves only if not attached
	if (blk != nullptr) 
		return;

	auto vlen = (vWpn.x < 0 ? -1 : 1) * vWpn.x;
	if (vlen > 0) {
		auto vcap = vlen + ACCEL * dt;
		if (vcap < 0)
			vcap = 0;
		vWpn.x *= vcap / vlen;
	}

	vWpn = vWpn + aWpn * dt;
	auto ds = vWpn * dt;
	cWeapon(data.mixed.gravity, ds);
}

void collision(float grav, vec2 ds) {
	CollResult res;
	//	x collision
	res = { false, ds.x };
	if (res.ds != 0) for (auto& block : map) {
		if (!(player.pos.y < block.pos.y + block.dim.h && player.pos.y + player.dim.h > block.pos.y))
			continue;

		if (ds.x > 0) {
			auto x1 = player.pos.x, x2 = player.pos.x + player.dim.w + res.ds;
			if (x1 < block.pos.x + block.dim.w && x2 > block.pos.x) {
				res = { true, block.pos.x - (player.pos.x + player.dim.w) };
				break;
			}
		}
		else {
			auto x1 = player.pos.x + res.ds, x2 = player.pos.x + player.dim.w;
			if (x1 < block.pos.x + block.dim.w && x2 > block.pos.x) {
				res = { true, (block.pos.x + block.dim.w) - player.pos.x };
				break;
			}
		}
	}
	player.pos.x += res.ds;
	if (res.collided) {
		accel.x = 0, speed.x = 0;
	}
	//	y 
	res = { false, ds.y };
	if (res.ds != 0) for (auto& block : map) {
		if (!(player.pos.x < block.pos.x + block.dim.w && player.pos.x + player.dim.w > block.pos.x))
			continue;

		if (ds.y > 0) {
			auto y1 = player.pos.y, y2 = player.pos.y + player.dim.h + res.ds;
			if (y1 < block.pos.y + block.dim.h && y2 > block.pos.y) {
				res = { true, block.pos.y - (player.pos.y + player.dim.h) };
				break;
			}
		}
		else {
			auto y1 = player.pos.y + res.ds, y2 = player.pos.y + player.dim.h;
			if (y1 < block.pos.y + block.dim.h && y2 > block.pos.y) {
				res = { true, (block.pos.y + block.dim.h) - player.pos.y };
				break;
			}
		}
	}

	player.pos.y += res.ds;
	if (ds.y > 0 && res.collided) {
		onGround = true;
		accel.y = 0, speed.y = 0;

		jumps = JUMPS;
	}
	else {
		onGround = false;
		accel.y = SGN_FALL grav;

		if (res.collided) {
			speed.y = 0;
		}
	}
}
void cWeapon(float grav, vec2 ds) {
	CollResult res;

	//	x collision
	res = { false, ds.x };
	if (res.ds != 0) for (auto it = map.begin(), end = map.end(); it != end; it++) {
		if (!(weapon.pos.y < it->pos.y + it->dim.h && weapon.pos.y + weapon.dim.h > it->pos.y))
			continue;

		if (ds.x > 0) {
			auto x1 = weapon.pos.x, x2 = weapon.pos.x + weapon.dim.w + res.ds;
			if (x1 < it->pos.x + it->dim.w && x2 > it->pos.x) {
				res = { true, it->pos.x - (weapon.pos.x + weapon.dim.w) };
				blk = it, side = LEFT;

				break;
			}
		}
		else {
			auto x1 = weapon.pos.x + res.ds, x2 = weapon.pos.x + weapon.dim.w;
			if (x1 < it->pos.x + it->dim.w && x2 > it->pos.x) {
				res = { true, (it->pos.x + it->dim.w) - weapon.pos.x };
				blk = it, side = RIGHT;

				break;
			}
		}
	}
	weapon.pos.x += res.ds;
	if (res.collided) {
		aWpn.x = 0, vWpn.x = 0;
	}

	//	y 
	res = { false, ds.y };
	if (res.ds != 0) for (auto it = map.begin(), end = map.end(); it != end; it++) {
		if (!(weapon.pos.x < it->pos.x + it->dim.w && weapon.pos.x + weapon.dim.w > it->pos.x))
			continue;

		if (ds.y > 0) {
			auto y1 = weapon.pos.y, y2 = weapon.pos.y + weapon.dim.h + res.ds;
			if (y1 < it->pos.y + it->dim.h && y2 > it->pos.y) {
				res = { true, it->pos.y - (weapon.pos.y + weapon.dim.h) };
				blk = it, side = UP;

				break;
			}
		}
		else {
			auto y1 = weapon.pos.y + res.ds, y2 = weapon.pos.y + weapon.dim.h;
			if (y1 < it->pos.y + it->dim.h && y2 > it->pos.y) {
				res = { true, (it->pos.y + it->dim.h) - weapon.pos.y };
				blk = it, side = DOWN;

				break;
			}
		}
	}

	weapon.pos.y += res.ds;
	if (ds.y > 0 && res.collided) {
		onGround = true;
		aWpn.y = 0, vWpn.y = 0;

		jumps = JUMPS;
	}
	else {
		onGround = false;
		aWpn.y = SGN_FALL grav;

		if (res.collided) {
			vWpn.y = 0;
		}
	}
}

void warp_wpn() {
	vec2 dir{ 1, 1 };
	player.pos = weapon.pos;

	switch (side)
	{
	case UP:
		player.pos.y = blk->pos.y - player.dim.h;
		if (speed.y > 0) speed.y *= -1;
		break; 
	case DOWN:
		player.pos.y = blk->pos.y + blk->dim.h;
		if (speed.y < 0) speed.y *= -1;
		break;

	case LEFT:
		player.pos.x = blk->pos.x - player.dim.w;
		if (speed.x > 0) speed.x *= -1;
		break;

	case RIGHT:
		player.pos.x = blk->pos.x + blk->dim.w;
		if (speed.x < 0) speed.x *= -1;

		break;

	default:
		break;
	}

}
void throw_wpn(float vel, vec2 dir) {
	weapon.pos = player.pos;
	vWpn = dir * vel;
	aWpn = { 0, data.mixed.gravity };
}

void keyUp(SDL_KeyboardEvent& event) {
	switch (event.keysym.scancode)
	{
	case SDL_SCANCODE_A:
		left = false;
		break;
	case SDL_SCANCODE_D:
		right = false;
		break;
	case SDL_SCANCODE_SPACE:
		jump = false;

	default:
		break;
	}
}
void keyDown(SDL_KeyboardEvent& event) {
	switch (event.keysym.scancode)
	{
	case SDL_SCANCODE_A:
		left = true;
		break;
	case SDL_SCANCODE_D:
		right = true;
		break;
	case SDL_SCANCODE_SPACE:
		jump = true;

	default:
		break;
	}
}

void mouseMov(SDL_MouseMotionEvent& event) {
}
void mouseUp(SDL_MouseButtonEvent& event) {
	switch (event.button)
	{
	case SDL_BUTTON_LEFT:
		mleft = false;
		break;
	case SDL_BUTTON_RIGHT:
		mright = false;
		break; 
	
	default:
		break;
	}
}
void mouseDown(SDL_MouseButtonEvent& event) {
	if (rWpn) {
		//	weapon already thrown
		switch (event.button)
		{
		case SDL_BUTTON_LEFT:
			//	warps player to weapon
			warp_wpn();
		case SDL_BUTTON_RIGHT:
			//	resets
			rWpn = false;
			blk = nullptr;
			side = NONE;

			break; 
		
		default:
			return;
		}
	}
	else {
		//	weapon not thrown yet
		float v = 0;
		switch (event.button)
		{
		case SDL_BUTTON_LEFT:
			if (!mleft) {
				mleft = true;
				v = 100;
			}
			break;
		case SDL_BUTTON_RIGHT:
			if (!mright) {
				mright = true;
				v = 200;
			}
			break; 
		
		default:
			return;
		}

		vec2 dir{ event.x, event.y };
		dir = dir / tileDim;
		dir = dir - player.pos;
		auto len = sqrtf(dir.x * dir.x + dir.y * dir.y);
		dir = dir / len;

		throw_wpn(v, dir);
		rWpn = true;
	}
}

void close() {
	run = false;
}
void events() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) switch (event.type)
	{
	case SDL_EVENT_QUIT:
		close();
		break;

	case SDL_EVENT_MOUSE_MOTION:
		mouseMov(event.motion);
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		mouseUp(event.button);
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		mouseDown(event.button);

	case SDL_EVENT_KEY_UP:
		keyUp(event.key);
		break;
	case SDL_EVENT_KEY_DOWN:
		keyDown(event.key);
		break;

	default:
		break;
	}
}
