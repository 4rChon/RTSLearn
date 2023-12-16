#include <Game/Map.h>
#include <fstream>

Map::Map() : size({0, 0}) { }

void Map::initialize(const Vec2i& size) {
    this->size = size;
    tilemap.resize(size.second, std::vector<std::shared_ptr<Tile>>(size.first));
}

void Map::set_tile(const Vec2i& pos, std::shared_ptr<Tile> tile) {
    tilemap[pos.second][pos.first] = tile;
}

std::weak_ptr<Tile> Map::get_tile(const Vec2i& pos) const {
    return tilemap[pos.second][pos.first];
}

const Vec2i& Map::get_size() const {
    return size;
}

std::vector<std::vector<int>> Map::get_pathable_map() const {
    std::vector<std::vector<int>> pathable_map(size.first, std::vector<int>(size.second, 0));
    for (int y = 0; y < size.second; ++y) {
        for (int x = 0; x < size.first; ++x) {
            if (tilemap[y][x]->is_pathable()) {
                pathable_map[x][y] = 1;
            }
        }
    }

    return pathable_map;
}
