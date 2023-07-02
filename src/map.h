#pragma once

#include "linm.h"

#include <initializer_list>
#include <memory>

extern Dim TileDim;

class Map
{
public:
	~Map() = default;

	Map() = default;
	Map(std::initializer_list<Entity> ents);

	bool collide(Entity target);

private:
	size_t _len;
	std::unique_ptr<Entity[]> _arr;
};

bool collision(Entity block, Entity target);
