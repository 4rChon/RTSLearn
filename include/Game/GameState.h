#pragma once
#include <TypeDefs.h>
#include <Game/UnitType.h>
#include <Game/TileType.h>
#include <Game/AbilityType.h>
#include <vector>

class GameState {
public:
    GameState(const vec2& map_size, int player_count);

    UnitType get_unit_type(const vec2& position) const;
    AbilityType get_unit_state(const vec2& position) const;
    int get_unit_health(const vec2& position) const;
    TileType get_tile_type(const vec2& position) const;
    int get_tile_minerals(const vec2& position) const;
    int get_player_vision(const vec2& position, int player_id) const;

    void set_unit_type(const vec2& position, UnitType type);
    void set_unit_state(const vec2& position, AbilityType state);
    void set_unit_health(const vec2& position, int health);
    void set_tile_type(const vec2& position, TileType type);
    void set_tile_minerals(const vec2& position, int minerals);
    void set_player_vision(const vec2& position, int player_id, int vision);

private:
    const int map_width;
    const int map_height;
    const int player_count;

    std::vector<UnitType> unit_types;
    std::vector<AbilityType> unit_states;
    std::vector<int> unit_health;
    std::vector<TileType> tile_types;
    std::vector<int> tile_minerals;
    std::vector<std::vector<int>> player_vision;
};