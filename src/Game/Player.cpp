#include <Game/Player.h>
#include <Game/Constants.h>

Player::Player(int id) 
    : id(id)
    , minerals(Constants::INITIAL_PLAYER_MINERALS)
    , supply(0)
    , max_supply(0)
    , selected_unit_id(-1) { }

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

void Player::modify_vision(const vec2& position, int value) {
    vision[position] += value;
}

bool Player::get_vision(const vec2& position) {
    return vision[position];
}
