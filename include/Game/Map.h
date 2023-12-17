#pragma once
#include <Game/Tile.h>
#include <TypeDefs.h>
#include <memory>

class Tile;

class Map {
public:
    Map(const std::string map_name);
    
    std::weak_ptr<Tile> get_tile(const Vec2i& pos) const;
    const Vec2i& get_size() const;
    bool has_line_of_sight(const Vec2i& start, const Vec2i& end) const;
    std::vector<std::vector<int>> get_pathable_map() const;
    std::vector<Vec2i> get_starting_locations() const;
private:
    std::vector<Vec2i> starting_locations;
    Vec2i size;
    std::vector<std::vector<std::shared_ptr<Tile>>> tilemap;
    std::vector<std::vector<int>> pathable_map;
    std::unordered_map<Vec2i, std::unordered_map<Vec2i, bool>> line_of_sight_cache;

    bool cast_sight_line(const Vec2i& start, const Vec2i& end) const;
    void set_tile(const Vec2i& pos, std::shared_ptr<Tile> tile);
};