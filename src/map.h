#pragma once

#include "linm.h"
#include "game.h"

#include <initializer_list>
#include <memory>

class Map
{
public:
	~Map() = default;

	Map() noexcept;
	Map(std::initializer_list<Entity> ents);

	size_t size() const noexcept;

	Entity* begin() noexcept;
	Entity* end() noexcept;

	bool collide(Entity target);

	Map(Map&&) noexcept = default;
	Map& operator=(Map&&) noexcept = default;

private:
	size_t _len;
	std::unique_ptr<Entity[]> _arr;
};

bool collision(Entity block, Entity target);
