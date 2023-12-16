#pragma once
#include <TypeDefs.h>
#include <unordered_map>
#include <vector>

class Pathfinder {
public:
	static std::vector<Vec2i> get_path(const Vec2i& start, const Vec2i& end, std::vector<std::vector<int>> map);
private:
	static int heuristic(const Vec2i& a, const Vec2i& b);
	static std::vector<Vec2i> reconstruct_path(std::unordered_map<Vec2i, Vec2i> came_from, Vec2i current);
	static Vec2i neighbours[4];
};