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
		mat2 mat;
		struct 
		{
			Pos pos;
			Dim dim;
		};

		SDL_FRect rect;
	};
};

void clear();
void present();

void setColor(uint8_t* color);
void renderFillRect(const Entity& rect);
