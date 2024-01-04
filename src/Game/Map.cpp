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
    line_of_sight_cache.resize(width * height, std::vector<bool>(width * height));
    for (auto y = 0; y < height; ++y) {
        for (auto x = 0; x < width; ++x) {
            for (auto y2 = 0; y2 < height; ++y2) {
                for (auto x2 = 0; x2 < width; ++x2) {
                    line_of_sight_cache[x + y * width][x2 + y2 * width] = cast_sight_line({ x, y }, { x2, y2 });
                }
            }
        }
    }

    pathable_map.resize(width * height);

    adjacent_tiles_cache.resize(width * height);
    for (auto y = 0; y < height; ++y) {
        for (auto x = 0; x < width; ++x) {
            auto& adjacent_tiles = adjacent_tiles_cache[x + y * width];
            if (x > 0) {
                adjacent_tiles[0] = tilemap[x - 1 + y * width].get();
            } else {
                adjacent_tiles[0] = nullptr;
            }

            if (y > 0) {
                adjacent_tiles[1] = tilemap[x + (y - 1) * width].get();
            }
            else {
                adjacent_tiles[1] = nullptr;
            }

            if (x < width - 1) {
                adjacent_tiles[2] = tilemap[x + 1 + y * width].get();
            }
            else {
                adjacent_tiles[2] = nullptr;
            }

            if (y < height - 1) {
                adjacent_tiles[3] = tilemap[x + (y + 1) * width].get();
            }
            else {
                adjacent_tiles[3] = nullptr;
            }
        }
    }
}

void Map::set_tile(const vec2& pos, std::unique_ptr<Tile> tile) {
    tilemap[pos.first + pos.second * size.first] = std::move(tile);
}

std::array<Tile*, 4> Map::get_adjacent_tiles(const vec2& pos) const {
    return adjacent_tiles_cache[pos.first + pos.second * size.first];
}

Tile* Map::get_tile(const vec2& pos) const {
    return tilemap[pos.first + pos.second * size.first].get();
}

const vec2& Map::get_size() const {
    return size;
}

bool Map::has_line_of_sight(const vec2& start, const vec2& end) const {
    return line_of_sight_cache[start.first + start.second * size.first][end.first + end.second * size.first];
}

bool Map::cast_sight_line(const vec2& start, const vec2& end) const {
    auto [x0, y0] = start;
    auto [x1, y1] = end;

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

Tile* Map::get_nearest_pathable_tile(const vec2& pos) const {
    for (const auto& neighbour : get_adjacent_tiles(pos)) {
        if (neighbour != nullptr && neighbour->is_pathable()) {
            return neighbour;
        }
    }

    return nullptr;
}
