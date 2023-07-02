#include "map.h"

Map::Map() noexcept 
	: _len{ 0 }, _arr{ nullptr } {
}
Map::Map(std::initializer_list<Entity> ents)
	: _len{ ents.size() }, _arr{ nullptr } {
	if (_len == 0)
		return;
		
	_arr = std::make_unique<Entity[]>(_len);

	size_t i = 0;
	for (auto it = ents.begin(), end = ents.end(); it != end; it++)
		_arr[i++] = std::move(*it);
}

bool Map::collide(Entity target) {
	for (auto it = _arr.get(), end = it + _len; it != end; it++) {
		auto res = collision(*it, target);
		if (res)
			return true;
	}

	return false;
}

size_t Map::size() const noexcept {
	return _len;
}
Entity* Map::begin() noexcept {
	return _arr.get();
}
Entity* Map::end() noexcept {
	return begin() + size();
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
