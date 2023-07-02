#include "game.h"

Pos camPos	= { 0, 0 };
Dim tileDim	= { 8, 8 };

Entity::Entity() noexcept
	: pos{ 0, 0 }, dim{ 0, 0 } {
}
Entity::Entity(mat2 mat) noexcept
	: mat{ mat } {
}
Entity::Entity(Pos pos, Dim dim) noexcept
	: pos{pos}, dim{dim} {
}
Entity::Entity(float x, float y, float w, float h) noexcept
	: pos{ x, y }, dim{ w, h } {
}

void clear() {
	SDL_RenderClear(App.renderer);
}
void present() {
	SDL_RenderPresent(App.renderer);
}

void setColor(uint8_t *color) {
	SDL_SetRenderDrawColor(App.renderer, color[0], color[1], color[2], color[3]);
}
void renderFillRect(const Entity& rect) {
	Entity tmp;
	tmp.pos = rect.pos * tileDim + camPos;
	tmp.dim = rect.dim * tileDim;

	SDL_RenderFillRect(App.renderer, (SDL_FRect*) &tmp);
}
