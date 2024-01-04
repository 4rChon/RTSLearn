#pragma once
#include <Game/Map.h>
#include <Game/PlayerInput.h>
#include <Game/Player.h>
#include <Game/UnitType.h>
#include <Game/Unit.h>
#include <memory>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <vector>

class Unit;
class Action;
class Player;
class Map;

class Game {
public:
    Game(unsigned short max_fps);
    void buffer_action(const PlayerInput& action);
    void step();
    void render();
    void load_map(const std::string& map_name);
    Map* get_map();
    void create_unit(UnitType unit_type, const vec2& position, int player_id);
    std::vector<ActionType> get_available_actions(int player_id);
    bool move_unit(Unit& unit, const vec2& target);
private:
    int ticks;

    std::shared_ptr<Map> map;
    std::stack<PlayerInput> input_buffer;

    std::vector<std::shared_ptr<Player>> players;
    std::unordered_map<int, std::shared_ptr<Unit>> units;
    std::unordered_set<std::shared_ptr<Unit>> acting_units;

    void destroy_unit(const Unit& unit);
    void init_player(int player_id, const vec2& starting_location);
    void enqueue_unit_action(std::shared_ptr<Unit> unit, std::unique_ptr<Action> action, bool replace_current_action);
};