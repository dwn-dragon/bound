#pragma once

#include "linm.h"
#include "app.h"

using Pos = vec2;
using Dim = vec2;

extern Pos camPos;
extern Dim tileDim;

struct Entity
{
	Entity() noexcept;
	Entity(mat2 mat) noexcept;
	Entity(Pos pos, Dim dim) noexcept;
	Entity(float x, float y, float w, float h) noexcept;

	union
	{
		struct 
		{
			Pos pos;
			Dim dim;
		};

		mat2 mat;
		float _raw[4];
		
		SDL_FRect rect;
	};
};

struct MixedData
{
	float limit, accel, decel;
	float gravity, vjump;
};
struct DampedData
{
	vec2 k;
	float accel, grav;
	float vjump;
};

struct Data
{
	MixedData mixed;
	DampedData damped;
};

void clear();
void present();

void setColor(uint8_t* color);
void renderFillRect(const Entity& rect);
