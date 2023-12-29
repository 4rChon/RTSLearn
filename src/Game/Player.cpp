#include <Game/Player.h>
#include <Game/Constants.h>
#include <Game/Map.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((b) < (a) ? (a) : (b))

Player::Player(int id, const vec2& map_size) 
    : id(id)
    , minerals(Constants::INITIAL_PLAYER_MINERALS)
    , supply(0)
    , max_supply(0)
    , selected_unit_id(-1) {
    vision.resize(map_size.first, std::vector<int>(map_size.second, 0));
}

int Player::get_id() const {
    return id;
}

int Player::get_minerals() const {
    return minerals;
}

int Player::get_free_supply() const {
    return max_supply - supply;
}

void Player::modify_minerals(int amount) {
    minerals += amount;
}

void Player::modify_supply(int amount) {
    supply += amount;
}

void Player::modify_max_supply(int amount) {
    max_supply += amount;
}

void Player::select_unit(int unit_id) {
    selected_unit_id = unit_id;
}

void Player::deselect_unit() {
    selected_unit_id = -1;
}

int Player::get_selected_unit_id() const {
    return selected_unit_id;
}

const std::string Player::get_resources_string() const {
    return std::to_string(id) + " :: Minerals: " + std::to_string(minerals) + " Supply: " + std::to_string(supply) + "/" + std::to_string(max_supply);
}

void Player::modify_vision(const Unit& unit, const Map& map, int value) {
    auto sight_range = Constants::unit_sight_range.at(unit.get_type());
    auto& unit_position = unit.get_position();
    const auto& [x_c, y_c] = unit_position;
    const auto& [x_max, y_max] = map.get_size();

    for (auto x = max(0, x_c - sight_range); x <= min(x_max - 1, x_c + sight_range); ++x) {
        for (auto y = max(0, y_c - sight_range); y <= min(y_max - 1, y_c + sight_range); ++y) {
            if (map.has_line_of_sight({x, y}, unit_position)) {
                vision[x][y] += value;
            }
        }
    }
}

bool Player::get_vision(const vec2& position) {
    return vision[position.first][position.second];
}
