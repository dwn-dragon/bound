#include "input.h"

#include <vector>
#include <fstream>
#include <iostream>

struct Range
{
	size_t start, end;
};

template< class Ty, class Fn >
inline Range _parse(Ty &&str, Fn &&fn, Range range) {
	Range res;
	//	starts at range start
	res.start = range.start;
	while (res.start < range.end) {
		//	keeps going until a valid character is found
		if (static_cast<bool>(fn(str[res.start])))
			break;
		//	increases after checks
		res.start++;
	}
	//	starts at word start
	res.end = res.start;
	while (res.end < range.end) {
		//	increases before checks
		res.end++;
		//	keeps going until an invalid character is found
		if (!static_cast<bool>(fn(str[res.end])))
			break;
	}
	//	returns res
	return res;
}

Map readMap(std::string path) {
	std::ifstream in;
	in.open(path);

	if (!in.is_open())
		return Map{ };

	std::vector<Entity> vec;

	while (!in.eof()) {
		//	reserves memory
		std::string line;
		line.reserve(11);
		//	reads line
		std::getline(in, line);
		if (line.size() == 0)
			continue;

		//	
		size_t i = 0;
		Range rem{ 0, line.length() };
		Entity row{ 0, 0, 0, 0 };

		while (true) {
			auto res = _parse(line, std::isgraph, rem);
			rem.start = res.end;

			auto len = res.end - res.start;
			if (len == 0)
				break;
			auto sub = line.substr(res.start, res.end - res.start);

			try {
				auto f = std::stof(sub);
				row._raw[i++] = f;
			}
			catch (const std::exception& err) {
				std::cerr << err.what() << std::endl;
			}
		}

		if (i < 4)
			throw std::exception{ "invalid file" };
		vec.push_back(std::move(row));
	}

	return Map{ vec.begin(), vec.end() };
}
