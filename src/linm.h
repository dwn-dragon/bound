#pragma once

#include <type_traits>
#include <SDL3/SDL.h>

class vec2
{
public:
	~vec2() noexcept = default;

	vec2() noexcept;
	vec2(float a, float b) noexcept;

	float& operator[](size_t len) noexcept;
	const float& operator[](size_t len) const noexcept;

	vec2 operator+() const noexcept;
	vec2 operator-() const noexcept;

	vec2 operator+(const float right) const noexcept;
	vec2 operator-(const float right) const noexcept;
	vec2 operator*(const float right) const noexcept;
	vec2 operator/(const float right) const noexcept;

	vec2 operator+(const vec2 right) const noexcept;
	vec2 operator-(const vec2 right) const noexcept;
	vec2 operator*(const vec2 right) const noexcept;
	vec2 operator/(const vec2 right) const noexcept;

	union
	{
		float _raw[2];
		union
		{
			struct
			{
				float x, y;
			};
			struct
			{
				float h, w;
			};
		};
	};
};

using Pos = vec2;
using Dim = vec2;

union Entity
{
	SDL_FRect rect;
	struct {
		Pos pos;
		Dim dim;
	};
};

template< class Fn >
inline vec2 apply_vec(Fn&& fn, const vec2& vec) {
	return vec2{
		fn( vec[0] ),
		fn( vec[1] )
	};
}
