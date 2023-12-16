#pragma once
#include <Game/ActionType.h>
#include <string>
#include <unordered_map>
#include <utility>

typedef std::pair<int, int> Vec2i;
typedef std::unordered_map<std::string, void*> Info;

// create a hash function for Vec2i
namespace std {
	template<>
	struct hash<Vec2i> {
		size_t operator()(const Vec2i& vec) const {
			return (size_t)vec.first << 32 | (unsigned int)vec.second;
		}
	};
}