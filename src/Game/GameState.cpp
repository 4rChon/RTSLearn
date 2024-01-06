#include <Game/GameState.h>

GameState::GameState(const vec2& map_size, const int player_count)
    : map_width(map_size.first)
    , map_height(map_size.second)
    , player_count(player_count)
{
    const auto vector_size = map_width * map_height;

    unit_types.resize(vector_size);
    unit_states.resize(vector_size);
    unit_health.resize(vector_size);

    tile_types.resize(vector_size);
    tile_minerals.resize(vector_size);

    player_vision.resize(player_count);
    for (auto& player_vision_vector : player_vision)
    {
        player_vision_vector.resize(vector_size);
    }
}

UnitType GameState::get_unit_type(const vec2& position) const
{
    const auto index = vec_to_index(position, map_width);
    return unit_types[index];
}

AbilityType GameState::get_unit_state(const vec2& position) const
{
    const auto index = vec_to_index(position, map_width);
    return unit_states[index];
}

int GameState::get_unit_health(const vec2& position) const
{
    const auto index = vec_to_index(position, map_width);
    return unit_health[index];
}

TileType GameState::get_tile_type(const vec2& position) const
{
    const auto index = vec_to_index(position, map_width);
    return tile_types[index];
}

int GameState::get_tile_minerals(const vec2& position) const
{
    const auto index = vec_to_index(position, map_width);
    return tile_minerals[index];
}

int GameState::get_player_vision(const vec2& position, int player_id) const
{
    const auto index = vec_to_index(position, map_width);
    return player_vision[player_id][index];
}

void GameState::set_unit_type(const vec2& position, UnitType type)
{
    const auto index = vec_to_index(position, map_width);
    unit_types[index] = type;
}

void GameState::set_unit_state(const vec2& position, AbilityType state)
{
    const auto index = vec_to_index(position, map_width);
    unit_states[index] = state;
}

void GameState::set_unit_health(const vec2& position, int health)
{
    const auto index = vec_to_index(position, map_width);
    unit_health[index] = health;
}

void GameState::set_tile_type(const vec2& position, TileType type)
{
    const auto index = vec_to_index(position, map_width);
    tile_types[index] = type;
}

void GameState::set_tile_minerals(const vec2& position, int minerals)
{
    const auto index = vec_to_index(position, map_width);
    tile_minerals[index] = minerals;
}

void GameState::set_player_vision(const vec2& position, int player_id, int vision)
{
    const auto index = vec_to_index(position, map_width);
    player_vision[player_id][index] = vision;
}
