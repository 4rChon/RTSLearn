#pragma once
#include <Game/Map.h>
#include <Game/PlayerInput.h>
#include <Game/Player.h>
#include <Game/UnitType.h>
#include <Game/Unit.h>
#include <memory>
#include <stack>
#include <vector>

class Unit;
class Action;
class Player;
class Map;
class Tile;

class Game : public std::enable_shared_from_this<Game> {
public:
    Game(unsigned short max_fps);
    void buffer_action(const PlayerInput& action);
    void step();
    void render();
    void load_map(const std::string& map_name);
    std::weak_ptr<Map> get_map();
    void create_unit(UnitType unit_type, const vec2& position, int player_id);
    void destroy_unit(const std::shared_ptr<Unit>& unit);
    std::weak_ptr<Tile> get_nearest_pathable_tile(const vec2& position);
    std::vector<ActionType> get_available_actions(int player_id);
    bool move_unit(std::shared_ptr<Unit> unit, const vec2& target);
private:
    int ticks;
    std::shared_ptr<Map> map;
    std::stack<PlayerInput> input_buffer;

    std::vector<std::shared_ptr<Player>> players;
    std::unordered_map<int, std::shared_ptr<Unit>> units;
    std::unordered_map<vec2, std::vector<std::vector<bool>>> line_of_sight_cache;

    void init_player(int player_id, const vec2& starting_location);
    void train(const PlayerInput& input, std::shared_ptr<Player> player, std::shared_ptr<Game> game, UnitType unit_type);
    void build(const PlayerInput& input, std::shared_ptr<Player> player, std::shared_ptr<Game> game, UnitType unit_type);
    bool select(std::shared_ptr<Unit> tile, std::shared_ptr<Player> player);
    bool move(std::shared_ptr<Unit> selected_unit, const PlayerInput& input, std::shared_ptr<Player> player, std::shared_ptr<Game> game);
    bool attack(std::shared_ptr<Unit> selected_unit, std::shared_ptr<Unit> target_unit, const PlayerInput& input, std::shared_ptr<Player> player, std::shared_ptr<Game> game);
    bool gather(std::shared_ptr<Unit> selected_unit, std::shared_ptr<Tile> target_tile, const PlayerInput& input, std::shared_ptr<Player> player, std::shared_ptr<Game> game);
};