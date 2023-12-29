#include <Game/Map.h>
#include <Game/Constants.h>
#include <Game/ResourceLoader.h>
#include <TypeDefs.h>
#include <memory>

Map::Map(const std::string map_name) {
    auto j = ResourceLoader::load_json(map_name);

    int width = j["dimensions"]["width"];
    int height = j["dimensions"]["height"];
    int players = j["players"];

    size = { width, height };
    tilemap.resize(width * height);

    starting_locations.reserve(players);

    // initialize tiles
    for (auto y = 0; y < height; ++y) {
        std::string line = j["tiles"][y];
        for (auto x = 0; x < width; ++x) {
            auto letter = line[x];
            auto tile = std::make_unique<Tile>(vec2{ x, y }, Constants::tile_letter.at(letter));
            set_tile({ x, y }, std::move(tile));
            if (letter == 'O') {
                starting_locations.push_back({ x, y });
            }
        }
    }

    // initialize line of sight cache
    line_of_sight_cache.resize(width, std::vector<std::vector<std::vector<bool>>>(height, std::vector<std::vector<bool>>(width, std::vector<bool>(height))));
    for (auto y = 0; y < height; ++y) {
        for (auto x = 0; x < width; ++x) {
            for (auto y2 = 0; y2 < height; ++y2) {
                for (auto x2 = 0; x2 < width; ++x2) {
                    line_of_sight_cache[x][y][x2][y2] = cast_sight_line({ x, y }, { x2, y2 });
                }
            }
        }
    }

    pathable_map.resize(width, std::vector<bool>(height));
}

void Map::set_tile(const vec2& pos, std::unique_ptr<Tile> tile) {
    tilemap[pos.first + pos.second * size.first] = std::move(tile);
}

Tile* Map::get_tile(const vec2& pos) const {
    return tilemap[pos.first + pos.second * size.first].get();
}

const vec2& Map::get_size() const {
    return size;
}

bool Map::has_line_of_sight(const vec2& start, const vec2& end) const {
    return line_of_sight_cache[start.first][start.second][end.first][end.second];
}

std::vector<std::vector<bool>> Map::get_pathable_map() {
    for (int x = 0; x < size.first; ++x) {
        for (int y = 0; y < size.second; ++y) {
            pathable_map[x][y] = tilemap[x + y * size.second]->is_pathable();
        }
    }

    return pathable_map;
}

bool Map::cast_sight_line(const vec2& start, const vec2& end) const {
    auto x0 = start.first;
    auto y0 = start.second;
    auto x1 = end.first;
    auto y1 = end.second;

    auto dx = abs(x1 - x0);
    auto sx = x0 < x1 ? 1 : -1;
    auto dy = -abs(y1 - y0);
    auto sy = y0 < y1 ? 1 : -1;
    auto err = dx + dy;

    while (true) {
        if (tilemap[x0 + y0 * size.first]->is_vision_blocker()) {
            return false;
        }

        if (x0 == x1 && y0 == y1) {
            return true;
        }

        auto e2 = 2 * err;
        if (e2 >= dy) {
            if (x0 == x1) {
                return true;
            }
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            if (y0 == y1) {
                return true;
            }
            err += dx;
            y0 += sy;
        }
    }
}

std::vector<vec2> Map::get_starting_locations() const {
    return starting_locations;
}

Tile* Map::get_nearest_pathable_tile(const vec2& pos) {
    auto& [x, y] = pos;
    auto& tile = tilemap[x + y * size.first];
    if (tile->is_pathable()) {
        return tile.get();
    }

    auto& tile_right = tilemap[x + 1 + y * size.first];
    if (tile_right->is_pathable()) {
        return tile_right.get();
    }

    auto& tile_left = tilemap[x - 1 + y * size.first];
    if (tile_left->is_pathable()) {
        return tile_left.get();
    }

    auto& tile_down = tilemap[x + (y + 1) * size.first];
    if (tile_down->is_pathable()) {
        return tile_down.get();
    }

    auto& tile_up = tilemap[x + (y - 1) * size.first];
    if (tile_up->is_pathable()) {
        return tile_up.get();
    }

    auto& tile_down_right = tilemap[x + 1 + (y + 1) * size.first];
    if (tile_down_right->is_pathable()) {
        return tile_down_right.get();
    }

    auto& tile_up_right = tilemap[x + 1 + (y - 1) * size.first];
    if (tile_up_right->is_pathable()) {
        return tile_up_right.get();
    }

    auto& tile_down_left = tilemap[x - 1 + (y + 1) * size.first];
    if (tile_down_left->is_pathable()) {
        return tile_down_left.get();
    }

    auto& tile_up_left = tilemap[x - 1 + (y - 1) & size.first];
    if (tile_up_left->is_pathable()) {
        return tile_up_left.get();
    }

    return nullptr;
}
