#include <Game/Constants.h>
#include <Game/Game.h>
#include <Game/PlayerInput.h>
#include <Game/Tile.h>
#include <Game/Unit.h>
#include <Game/UnitType.h>
#include <Game/Actions/TrainWorker.h>
#include <Game/Actions/Noop.h>
#include <Game/Actions/Move.h>
#include <TypeDefs.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>
#include <sstream>
#include <iostream>

Game::Game() 
    : map(std::make_shared<Map>())
    , ticks(0) {}

void Game::buffer_action(const PlayerInput& input) {
    input_buffer.push(input);
}

void Game::step() {
    while (!input_buffer.empty()) {
        const PlayerInput& input = input_buffer.top();
        input_buffer.pop();

        auto &player = players[input.player_id];
        auto game = std::weak_ptr<Game>(shared_from_this());
        auto selected_unit_id = player->get_selected_unit_id();
        std::shared_ptr<Action> action;
        switch (input.type) {
            case ActionType::Noop:
                break;
            case ActionType::Click: {
                // we need to perform a context dependent action
                auto tile = map->get_tile(input.target).lock();
                auto tile_unit = tile->get_unit().lock();
                
                auto x = tile->get_position().first;
                auto y = tile->get_position().second;

                // if the target is a friendly unit, select it
                if (tile_unit && tile_unit->get_owner() == input.player_id) {
                    player->select_unit(tile_unit->get_id());

                    break;
                }
                
                // if the target is an enemy unit, check if we have a unit selected, if so, attack it
                //if (tile_unit && tile_unit->get_owner() != input.player_id) {
                //	auto unit = player->get_selected_unit().lock();
                //	if (unit) {
                //		auto action = std::make_shared<Action>(Attack(input.target, actor, player, game));
                //		unit->enqueue_action(action);
                //	}
                //	break;
                //}

                // if the target is a pathable tile, check if we have a unit selected, if so, move to it
                if (tile->is_pathable()) {
                    auto unit_id = player->get_selected_unit_id();
                    if (unit_id < 0) {
                        break;
                    }

                    auto& unit = units[unit_id];
                    if (Constants::unit_move_cooldown.at(unit->get_type()) > 0) {
                        auto action = std::make_unique<Move>(Move(input.target, unit, player, game));
                        unit->enqueue_action(std::move(action), true);
                    }
                    break;
                }
                break;
            }
            case ActionType::TrainWorker: {
                if (selected_unit_id < 0) {
                    break;
                }

                if (units.find(selected_unit_id) == units.end()) {
                    break;
                }

                auto& unit = units[selected_unit_id];
                auto action = std::make_unique<TrainWorker>(TrainWorker(input.target, unit, player, game));
                unit->enqueue_action(std::move(action), true);
                break;
            }
            case ActionType::TrainMeleeWarrior: {
                break;
            }
            case ActionType::TrainRangedWarrior: {
                break;
            }
            case ActionType::BuildFarm: {
                break;
            }
            case ActionType::BuildBarracks: {
                break;
            }
            case ActionType::BuildTownHall: {
                break;
            }
        }
    }

    for (auto& unit_kv : units) {
        unit_kv.second->act();
    }

    ticks++;
}

void Game::render() {
    std::stringstream ss;
    ss << "\x1b[m\x1b[2J";

    auto& size = map->get_size();
    auto x_max = size.second;
    auto y_max = size.first;
    for (auto y = 0; y < y_max; ++y) {
        for (auto x = 0; x < x_max; ++x) {
            if (auto tile_ptr = map->get_tile({ x, y }).lock()) {
                ss << tile_ptr->get_sprite();
            }
        }

        ss << "\x1b[m\n";
    }

    for (const auto& player : players) {
        ss << player->get_resources_string() << '\n';
    }

    ss << "Ticks: " << ticks << '\n';

    std::cout << ss.str();
}

void Game::load_map(const std::string& map_name) {
    std::ifstream file;
    file.open(map_name);

    if (file.fail()) {
        perror(map_name.c_str());
        exit(1);
    }

    nlohmann::json j = nlohmann::json::parse(file);

    int width = j["dimensions"]["width"];
    int height = j["dimensions"]["height"];
    int players = j["players"];
    map->initialize({ width, height });

    std::vector<Vec2i> starting_locations;

    for (auto y = 0; y < height; ++y) {
        std::string line = j["tiles"][y];
        for (auto x = 0; x < width; ++x) {
            auto letter = line[x];

            map->set_tile({ x, y }, Tile::create_tile({ x, y }, letter));
            if (letter == 'O') {
                starting_locations.push_back({ x, y });
            }
        }
    }

    for (auto i = 0; i < players; ++i) {
        init_player(i, starting_locations[i]);
    }

    file.close();
}

std::weak_ptr<Map> Game::get_map() {
    return map;
}

void Game::init_player(int player_id, const Vec2i& starting_location) {
    auto player = std::make_shared<Player>(player_id);

    players.push_back(player);

    create_unit(UnitType::TownHall, starting_location, player_id);
}

void Game::create_unit(UnitType unit_type, const Vec2i& position, int player_id) {
    auto unit = std::make_shared<Unit>(unit_type, position, player_id);
    units.insert({ unit->get_id(), unit });
    map->get_tile(position).lock()->set_unit(unit);

    players[player_id]->modify_max_supply(Constants::unit_supply_provided.at(unit_type));
}

void Game::destroy_unit(const std::shared_ptr<Unit>& unit) {
    units.erase(unit->get_id());
    map->get_tile(unit->get_position()).lock()->unset_unit();

    auto unit_type = unit->get_type();

    players[unit->get_owner()]->modify_supply(Constants::unit_supply_cost.at(unit_type));
    players[unit->get_owner()]->modify_max_supply(-Constants::unit_supply_provided.at(unit_type));
}

std::weak_ptr<Tile> Game::get_nearest_pathable_tile(const Vec2i& position) {
    auto tile = map->get_tile(position).lock();
    if (tile->is_pathable()) {
        return tile;
    }

    auto x_max = map->get_size().second;
    auto y_max = map->get_size().first;

    // check cardinal directions
    auto tile_right = map->get_tile({ position.first + 1, position.second }).lock();
    if (tile_right->is_pathable()) {
        return tile_right;
    }
    
    auto tile_left = map->get_tile({ position.first - 1, position.second }).lock();
    if (tile_left->is_pathable()) {
        return tile_left;
    }
    
    auto tile_down = map->get_tile({ position.first, position.second + 1 }).lock();
    if (tile_down->is_pathable()) {
        return tile_down;
    }
    
    auto tile_up = map->get_tile({ position.first, position.second - 1 }).lock();
    if (tile_up->is_pathable()) {
        return tile_up;
    }

    // check diagonals
    auto tile_down_right = map->get_tile({ position.first + 1, position.second + 1 }).lock();
    if (tile_down_right->is_pathable()) {
        return tile_down_right;
    }

    auto tile_up_right = map->get_tile({ position.first + 1, position.second - 1 }).lock();
    if (tile_up_right->is_pathable()) {
        return tile_up_right;
    }

    auto tile_down_left = map->get_tile({ position.first - 1, position.second + 1 }).lock();
    if (tile_down_left->is_pathable()) {
        return tile_down_left;
    }

    auto tile_up_left = map->get_tile({ position.first - 1, position.second - 1 }).lock();
    if (tile_up_left->is_pathable()) {
        return tile_up_left;
    }

    return std::weak_ptr<Tile>();
}

std::vector<ActionType> Game::get_available_actions(int player_id) {
    auto selected_unit = players[player_id]->get_selected_unit_id();
    if (selected_unit < 0) {
        return { ActionType::Noop, ActionType::Click };
    }

    return units[selected_unit]->get_available_actions();
}

bool Game::move_unit(std::shared_ptr<Unit> unit, const Vec2i& target) {
    if (!map->get_tile(target).lock()->is_pathable()) {
        return false;
    }

    map->get_tile(unit->get_position()).lock()->unset_unit();
    unit->set_position(target);
    map->get_tile(target).lock()->set_unit(unit);

    return true;
}
