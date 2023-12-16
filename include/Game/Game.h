#pragma once
#include <Game/Map.h>
#include <Game/PlayerInput.h>
#include <Game/Player.h>
#include <Game/UnitType.h>
#include <Game/Unit.h>
#include <memory>
#include <stack>
#include <unordered_set>
#include <vector>
#include <chrono>
class Unit;
class Action;
class Player;
class Map;
class Tile;

class Game : public std::enable_shared_from_this<Game> {
public:
    Game();
    void buffer_action(const PlayerInput& action);
    void step();
    void render();
    void load_map(const std::string& map_name);
    std::weak_ptr<Map> get_map();
    void create_unit(UnitType unit_type, const Vec2i& position, int player_id);
    void destroy_unit(const std::shared_ptr<Unit>& unit);
    std::weak_ptr<Tile> get_nearest_pathable_tile(const Vec2i& position);
    std::vector<ActionType> get_available_actions(int player_id);
    bool move_unit(std::shared_ptr<Unit> unit, const Vec2i& target);
private:
    int ticks;
    std::shared_ptr<Map> map;
    std::stack<PlayerInput> input_buffer;

    std::vector<std::shared_ptr<Player>> players;
    std::unordered_map<int, std::shared_ptr<Unit>> units;

    void init_player(int player_id, const Vec2i& starting_location);
};