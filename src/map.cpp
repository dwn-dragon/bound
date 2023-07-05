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

size_t Map::size() const noexcept {
	return _len;
}
Entity* Map::begin() noexcept {
	return _arr.get();
}
Entity* Map::end() noexcept {
	return begin() + size();
}
