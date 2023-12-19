#include <Game/Constants.h>
#include <Game/Game.h>
#include <Game/PlayerInput.h>
#include <Game/Tile.h>
#include <Game/Unit.h>
#include <Game/UnitType.h>
#include <Game/Actions/Attack.h>
#include <Game/Actions/Train.h>
#include <Game/Actions/Noop.h>
#include <Game/Actions/Move.h>
#include <Game/Actions/Gather.h>
#include <Game/Actions/Build.h>
#include <Game/Renderer.h>
#include <TypeDefs.h>
#include <memory>
#include <sstream>
#include <iostream>
#include <chrono>

Game::Game(unsigned short max_fps) 
    : map(nullptr)
    , ticks(0) {
    Renderer::init(max_fps);
}

void Game::buffer_action(const PlayerInput& input) {
    input_buffer.push(input);
}

void Game::step() {
    while (!input_buffer.empty()) {
        const PlayerInput& input = input_buffer.top();
        input_buffer.pop();

        auto& player = players[input.player_id];
        auto game = std::weak_ptr<Game>(shared_from_this());
        auto selected_unit_id = player->get_selected_unit_id();
        switch (input.type) {
            case ActionType::Noop:
                break;
            case ActionType::Click: {
                auto target_tile = map->get_tile(input.target).lock();
                auto target_unit = target_tile->get_unit().lock();

                if (select(target_unit, player)) {
                    break;
                }
                
                if (selected_unit_id < 0 || !units.contains(selected_unit_id)) {
                    break;
                }

                auto& selected_unit = units[selected_unit_id];
                if (!selected_unit || !selected_unit->is_alive()) {
                    break;
                }

                if (attack(selected_unit, target_unit, input, player, game)) {
                    break;
                }

                if (!target_tile->is_pathable()) {
                    break;
                }

                if (gather(selected_unit, target_tile, input, player, game)) {
                    break;
                }

                move(selected_unit, input, player, game);
                break;
            }
            case ActionType::TrainWorker: {
                train(input, player, game, selected_unit_id, UnitType::Worker);
                break;
            }
            case ActionType::TrainMeleeWarrior: {
                train(input, player, game, selected_unit_id, UnitType::MeleeWarrior);
                break;
            }
            case ActionType::TrainRangedWarrior: {
                train(input, player, game, selected_unit_id, UnitType::RangedWarrior);
                break;
            }
            case ActionType::BuildFarm: {
                build(input, player, game, selected_unit_id, UnitType::Farm);
                break;
            }
            case ActionType::BuildBarracks: {
                build(input, player, game, selected_unit_id, UnitType::Barracks);
                break;
            }
            case ActionType::BuildTownHall: {
                build(input, player, game, selected_unit_id, UnitType::TownHall);
                break;
            }
        }
    }

    for (auto& unit_kv : units) {
        unit_kv.second->act();
    }

    for (auto& unit_kv : units) {
        if (!unit_kv.second->is_alive()) {
            destroy_unit(unit_kv.second);
        }
    }

    ++ticks;
}

void Game::render() {
    Renderer::render(ticks, players, map);
}

void Game::load_map(const std::string& map_name) {
    map = std::make_shared<Map>(map_name);
    auto starting_locations = map->get_starting_locations();
    for (auto i = 0; i < starting_locations.size(); ++i) {
        init_player(i, starting_locations[i]);
    }
}

std::weak_ptr<Map> Game::get_map() {
    return map;
}

void Game::init_player(int player_id, const Vec2i& starting_location) {
    auto player = std::make_shared<Player>(player_id);

    players.push_back(player);

    create_unit(UnitType::TownHall, starting_location, player_id);
}

void Game::train(const PlayerInput& input, std::weak_ptr<Player> player, std::weak_ptr<Game> game, int selected_unit_id, UnitType unit_type) {
    if (selected_unit_id < 0) {
        return;
    }

    if (!units.contains(selected_unit_id)) {
        return;
    }

    auto& unit = units[selected_unit_id];
    auto& unit_abilities = Constants::unit_abilities.at(unit->get_type());
    if (unit_abilities.contains((AbilityType)input.type)) {
        auto action = std::make_unique<Train>(Train(input.target, unit, player, game, unit_type));
        unit->enqueue_action(std::move(action), true);
    }
}

void Game::build(const PlayerInput& input, std::weak_ptr<Player> player, std::weak_ptr<Game> game, int selected_unit_id, UnitType unit_type) {
    if (selected_unit_id < 0) {
        return;
    }

    if (!units.contains(selected_unit_id)) {
        return;
    }

    auto& unit = units[selected_unit_id];
    auto& unit_abilities = Constants::unit_abilities.at(unit->get_type());
    if (unit_abilities.contains((AbilityType)input.type)) {
        auto action = std::make_unique<Build>(input.target, unit, player, game, unit_type);
        unit->enqueue_action(std::move(action), true);
    }
}

bool Game::select(std::shared_ptr<Unit> unit, std::shared_ptr<Player> player) {
    if (!unit || unit->get_owner() != player->get_id()) {
        return false;
    }

    player->select_unit(unit->get_id());

    return false;
}

bool Game::move(std::shared_ptr<Unit> selected_unit, const PlayerInput& input, std::weak_ptr<Player> player, std::weak_ptr<Game> game) {
    auto& unit_abilities = Constants::unit_abilities.at(selected_unit->get_type());
    if (!unit_abilities.contains(AbilityType::Move)) {
        return false;
    }

    auto action = std::make_unique<Move>(Move(input.target, selected_unit, player, game));
    selected_unit->enqueue_action(std::move(action), true);
}

bool Game::attack(std::shared_ptr<Unit> selected_unit, std::shared_ptr<Unit> target_unit, const PlayerInput& input, std::weak_ptr<Player> player, std::weak_ptr<Game> game) {
    if (!target_unit || target_unit->get_owner() == input.player_id) {
        return false;
    }
 
    auto& unit_abilities = Constants::unit_abilities.at(selected_unit->get_type());
    if (!unit_abilities.contains(AbilityType::Attack)) {
        return false;
    }

    auto action = std::make_unique<Attack>(input.target, selected_unit, player, game);
    selected_unit->enqueue_action(std::move(action), true);

    return true;
}

bool Game::gather(std::shared_ptr<Unit> selected_unit, std::shared_ptr<Tile> target_tile, const PlayerInput& input, std::weak_ptr<Player> player, std::weak_ptr<Game> game) {
    auto& unit_abilities = Constants::unit_abilities.at(selected_unit->get_type());
    if (!unit_abilities.contains(AbilityType::Gather) || target_tile->get_type() != TileType::Mine) {
        return false;
    }

    auto action = std::make_unique<Gather>(input.target, selected_unit, player, game);
    selected_unit->enqueue_action(std::move(action), true);

    return true;
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

    players[unit->get_owner()]->modify_supply(-Constants::unit_supply_cost.at(unit_type));
    players[unit->get_owner()]->modify_max_supply(-Constants::unit_supply_provided.at(unit_type));
}

std::weak_ptr<Tile> Game::get_nearest_pathable_tile(const Vec2i& position) {
    auto tile = map->get_tile(position).lock();
    if (tile->is_pathable()) {
        return tile;
    }

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
    auto selected_unit_id = players[player_id]->get_selected_unit_id();
    if (selected_unit_id < 0) {
        return { ActionType::Noop, ActionType::Click };
    }

    if (!units.contains(selected_unit_id)) {
        return { ActionType::Noop, ActionType::Click };
    }

    auto& unit = units[selected_unit_id];
    if (!unit || !unit->is_alive()) {
        return { ActionType::Noop, ActionType::Click };
    }

    return unit->get_available_actions();
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
