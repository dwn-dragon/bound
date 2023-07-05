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

	template< class It >
	Map(It begin, It end);

	size_t size() const noexcept;

	Entity* begin() noexcept;
	Entity* end() noexcept;

	Map(Map&&) noexcept = default;
	Map& operator=(Map&&) noexcept = default;

private:
	size_t _len;
	std::unique_ptr<Entity[]> _arr;
};

template <class It>
inline Map::Map(It begin, It end) 
	: _len{ static_cast<size_t>(end - begin) } {
	_arr = std::make_unique<Entity[]>(_len);

	size_t i = 0;
	for (auto it = begin; it != end; it++) {
		_arr[i++] = std::move(*it);
	}
}
