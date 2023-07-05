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

CollResult Map::coll_x(Entity entity, float v) {
	return _coll_apply(&Map::_coll_x, entity, v);
}
CollResult Map::coll_y(Entity entity, float v) {
	return _coll_apply(&Map::_coll_y, entity, v);
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

CollResult Map::_coll_apply(CollResult(Map::* fn)(Entity, Entity, float), Entity target, float v) {
	if (v == 0)
		return { false, 0 };

	for (auto it = begin(); it != end(); it++) {
		auto res = (this->*fn)(*it, target, v);
		if (res.collided)
			return res;
	}

	return { false, v };
}
CollResult Map::_coll_x(Entity block, Entity target, float ds) {
	//	checks on the unmoved ax
	auto r1 = target.pos.y <= block.pos.y + block.dim.y
		&& target.pos.y + target.dim.y >= block.pos.y;
	if (!r1)
		return { false, ds };

	//	checks for the moved ax
	float x1 = 0, x2 = 0;
	if (ds > 0) {
		x1 = target.pos.x, x2 = target.pos.x + target.dim.x + ds;
		if (!(x1 <= block.pos.x + block.dim.x && x2 >= block.pos.x))
			return { false, ds };

		auto rs = block.pos.x - (target.pos.x + target.dim.w);
		return { true, rs };
	}
	else {
		x1 = target.pos.x + ds, x2 = target.pos.x + target.dim.x;
		if (!(x1 <= block.pos.x + block.dim.x && x2 >= block.pos.x))
			return { false, ds };
			
		auto rs = block.pos.x + block.dim.w - target.pos.x;
		return { true, rs };
	}

}
CollResult Map::_coll_y(Entity block, Entity target, float ds) {
	//	checks on the unmoved ax
	auto r1 = target.pos.x <= block.pos.x + block.dim.x
		&& target.pos.x + target.dim.x >= block.pos.x;
	if (!r1)
		return { false, ds };

	//	checks for the moved ax
	float y1 = 0, y2 = 0;
	if (ds > 0) {
		y1 = target.pos.y, y2 = target.pos.y + target.dim.y + ds;
		if (!(y1 <= block.pos.y + block.dim.y && y2 >= block.pos.y))
			return { false, ds };
			
		auto rs = block.pos.y - (target.pos.y + target.dim.h);
		return { true, rs };
	}
	else {
		y1 = target.pos.y + ds, y2 = target.pos.y + target.dim.y;
		if (!(y1 <= block.pos.y + block.dim.y && y2 >= block.pos.y))
			return { false, ds };

		auto rs = block.pos.y + block.dim.h - target.pos.y;
		return { true, rs };
	}
}
