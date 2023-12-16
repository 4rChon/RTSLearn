#pragma once
#include <Game/Tile.h>
#include <TypeDefs.h>
#include <memory>

class Tile;

class Map {
public:
    Map();
    void initialize(const Vec2i& size);
    void set_tile(const Vec2i& pos, std::shared_ptr<Tile> tile);
    std::weak_ptr<Tile> get_tile(const Vec2i& pos) const;
    const Vec2i& get_size() const;
    std::vector<std::vector<int>> get_pathable_map() const;
private:
    Vec2i size;
    std::vector<std::vector<std::shared_ptr<Tile>>> tilemap;
};