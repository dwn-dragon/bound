#include "linm.h"

vec2::vec2() noexcept
	: _raw{ 0, 0 } {
}
vec2::vec2(float a, float b) noexcept
	: _raw{ a, b } {
}

float &vec2::operator[](size_t len) noexcept {
	return _raw[len];
}
const float &vec2::operator[](size_t len) const noexcept {
	return _raw[len];
}

vec2 vec2::operator+() const noexcept {
	return *this;
}
vec2 vec2::operator-() const noexcept {
	return vec2{
		-_raw[0],
		-_raw[1]
	};
}

vec2 vec2::operator+(const float right) const noexcept {
	return vec2{
		_raw[0] + right,
		_raw[1] + right
	};
}
vec2 vec2::operator-(const float right) const noexcept {
	return vec2{
		_raw[0] - right,
		_raw[1] - right
	};
}
vec2 vec2::operator*(const float right) const noexcept {
	return vec2{
		_raw[0] * right,
		_raw[1] * right
	};
}
vec2 vec2::operator/(const float right) const noexcept {
	return vec2{
		_raw[0] / right,
		_raw[1] / right
	};
}

vec2 vec2::operator+(const vec2 right) const noexcept {
	return vec2{
		_raw[0] + _raw[0],
		_raw[1] + _raw[1]
	};
}
vec2 vec2::operator-(const vec2 right) const noexcept {
	return vec2{
		_raw[0] - _raw[0],
		_raw[1] - _raw[1]
	};
}
vec2 vec2::operator*(const vec2 right) const noexcept {
	return vec2{
		_raw[0] * _raw[0],
		_raw[1] * _raw[1]
	};
}
vec2 vec2::operator/(const vec2 right) const noexcept {
	return vec2{
		_raw[0] / _raw[0],
		_raw[1] / _raw[1]
	};
}
