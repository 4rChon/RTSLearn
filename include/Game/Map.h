#pragma once
#include <Game/Tile.h>
#include <TypeDefs.h>
#include <memory>

class Tile;

class Map {
public:
    Map(const std::string map_name);
    
    Tile* get_tile(const vec2& pos) const;
    Tile* get_nearest_pathable_tile(const vec2& pos);
    const vec2& get_size() const;
    bool has_line_of_sight(const vec2& start, const vec2& end) const;
    std::vector<std::vector<bool>> get_pathable_map();
    std::vector<vec2> get_starting_locations() const;
private:
    std::vector<vec2> starting_locations;
    vec2 size;
    std::vector<std::unique_ptr<Tile>> tilemap;
    std::vector<std::vector<bool>> pathable_map;
    std::vector<std::vector<std::vector<std::vector<bool>>>> line_of_sight_cache;

    bool cast_sight_line(const vec2& start, const vec2& end) const;
    void set_tile(const vec2& pos, std::unique_ptr<Tile> tile);
};