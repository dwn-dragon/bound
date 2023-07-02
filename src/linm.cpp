#include "linm.h"

vec2::vec2() noexcept
	: _raw{ 0, 0 } {
}
vec2::vec2(float a, float b) noexcept
	: _raw{ a, b } {
}

float &vec2::operator[](size_t len) {
	return _raw[len];
}
const float &vec2::operator[](size_t len) const {
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
		_raw[0] + right._raw[0],
		_raw[1] + right._raw[1]
	};
}
vec2 vec2::operator-(const vec2 right) const noexcept {
	return vec2{
		_raw[0] - right._raw[0],
		_raw[1] - right._raw[1]
	};
}
vec2 vec2::operator*(const vec2 right) const noexcept {
	return vec2{
		_raw[0] * right._raw[0],
		_raw[1] * right._raw[1]
	};
}
vec2 vec2::operator/(const vec2 right) const noexcept {
	return vec2{
		_raw[0] / right._raw[0],
		_raw[1] / right._raw[1]
	};
}

mat2::mat2(const vec2 v1, const vec2 v2) noexcept 
	: _cols{ v1, v2 }{
}

vec2& mat2::operator[](size_t cols) {
	return _cols[cols];
}
const vec2& mat2::operator[](size_t cols) const {
	return _cols[cols];
}

vec2 mat2::operator*(const vec2 right) const noexcept {
	return vec2{
		(*this)[0][0] * right[0] + (*this)[1][0] * right[1],
		(*this)[0][1] * right[0] + (*this)[1][1] * right[1]
	};
}
mat2 mat2::operator*(const mat2 right) const noexcept {
	return mat2{
		*this * right[0],
		*this * right[1]
	};
}
