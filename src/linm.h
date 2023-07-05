#pragma once

#include <type_traits>
#include <SDL3/SDL.h>

class vec2
{
public:
	~vec2() noexcept = default;

	vec2() noexcept;
	vec2(float a, float b) noexcept;

	float& operator[](size_t len);
	const float& operator[](size_t len) const;

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
				float w, h;
			};
		};
	};
};

class mat2
{
public:
	~mat2() noexcept = default;
	
	mat2() noexcept = default;
	mat2(const vec2 v1, const vec2 v2) noexcept;

	vec2& operator[](size_t cols);
	const vec2& operator[](size_t cols) const;

	vec2 operator*(const vec2 right) const noexcept;
	mat2 operator*(const mat2 right) const noexcept;

private:
	vec2 _cols[2];
};

#include "linm.hpp"
