#include "map.h"

Dim TileDim = { 16, 16 };

Map::Map(std::initializer_list<Entity> ents)
	: _len{ ents.size() } {
	if (_len == 0)
		return;

	_arr = std::make_unique<Entity[]>(_len);
	
}
bool Map::collide(Entity target) {
	for (auto it = _arr.get(), end = it + _len; it != end; it++)
		if (collision(*it, target))
			return true;
	return false;
}

bool collision(Entity block, Entity target) {
	bool rx = 
		target.pos.x <= block.pos.x + block.dim.x &&
		target.pos.x + target.dim.x >= block.pos.x; 
	bool ry =
		target.pos.y <= block.pos.y + block.dim.y &&
		target.pos.y + target.dim.y >= block.pos.y; 

	return rx && ry;
}
