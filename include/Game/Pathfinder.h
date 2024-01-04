#pragma once
#include <TypeDefs.h>
#include <Game/Map.h>
#include <unordered_map>
#include <vector>

namespace Pathfinder{
    void get_path(const vec2& start, const vec2& end, const Map& map, std::vector<vec2>& out_path);
}

namespace {
    int heuristic(const vec2& a, const vec2& b);
}