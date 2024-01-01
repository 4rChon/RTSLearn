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
#include <Game/Pathfinder.h>
#include <Game/Renderer/Renderer.h>
#include <TypeDefs.h>
#include <memory>
#include <string>

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
        auto selected_unit_id = player->get_selected_unit_id();
        auto selected_unit = selected_unit_id < 0 || !units.contains(selected_unit_id) || !units[selected_unit_id]->is_alive() ? nullptr : units[selected_unit_id];
        auto target_tile = map->get_tile(input.target);
        auto target_unit = target_tile->get_unit();

        switch (input.type) {
            case ActionType::Noop:
                break;
            case ActionType::Click: {
                if (target_unit && target_unit->get_owner() == player->get_id()) {
                    player->select_unit(target_unit->get_id());

                    break;
                }

                if (Attack::can_act(selected_unit.get(), target_unit, input.player_id)) {
                    auto action = std::make_unique<Attack>(input.target, *selected_unit, *player, *this);
                    enqueue_unit_action(selected_unit, std::move(action), true);

                    break;
                }

                if (Gather::can_act(selected_unit.get(), target_tile)) {
                    auto action = std::make_unique<Gather>(input.target, *selected_unit, *player, *this);
                    enqueue_unit_action(selected_unit, std::move(action), true);

                    break;
                }

                if (Move::can_act(selected_unit.get(), target_tile)) {
                    auto action = std::make_unique<Move>(input.target, *selected_unit, *player, *this);
                    enqueue_unit_action(selected_unit, std::move(action), true);
                }

                break;
            }
            case ActionType::TrainWorker: {
                if (Train::can_act(selected_unit.get(), input.type)) {
                    auto action = std::make_unique<Train>(input.target, *selected_unit, *player, *this, UnitType::Worker);
                    enqueue_unit_action(selected_unit, std::move(action), true);
                }
                break;
            }
            case ActionType::TrainMeleeWarrior: {
                if (Train::can_act(selected_unit.get(), input.type)) {
                    auto action = std::make_unique<Train>(input.target, *selected_unit, *player, *this, UnitType::MeleeWarrior);
                    enqueue_unit_action(selected_unit, std::move(action), true);
                }
                break;
            }
            case ActionType::TrainRangedWarrior: {
                if (Train::can_act(selected_unit.get(), input.type)) {
                    auto action = std::make_unique<Train>(input.target, *selected_unit, *player, *this, UnitType::RangedWarrior);
                    enqueue_unit_action(selected_unit, std::move(action), true);
                }
                break;
            }
            case ActionType::BuildFarm: {
                if (Build::can_act(selected_unit.get(), input.type)) {
                    auto action = std::make_unique<Build>(input.target, *selected_unit, *player, *this, UnitType::Farm);
                    enqueue_unit_action(selected_unit, std::move(action), true);
                }
                break;
            }
            case ActionType::BuildBarracks: {
                if (Build::can_act(selected_unit.get(), input.type)) {
                    auto action = std::make_unique<Build>(input.target, *selected_unit, *player, *this, UnitType::Barracks);
                    enqueue_unit_action(selected_unit, std::move(action), true);
                }
                break;
            }
            case ActionType::BuildTownHall: {
                if (Build::can_act(selected_unit.get(), input.type)) {
                    auto action = std::make_unique<Build>(input.target, *selected_unit, *player, *this, UnitType::TownHall);
                    enqueue_unit_action(selected_unit, std::move(action), true);
                }
                break;
            }
        }
    }

    for (auto& [id, unit] : units) {
        if (unit->is_alive()) {
            unit->act();
        }
    }
    // TODO: Only iterate over units that are currently acting
    // units can enqueue actions within actions, so we need to change some internals
    //for (auto it = acting_units.begin(); it != acting_units.end();) {
    //    auto unit = *it;
    //    if (!unit->is_alive() || unit->act() != ActionResult::Running) {
    //        it = acting_units.erase(it);
    //    }
    //    else {
    //        ++it;
    //    }
    //}

    for (auto it = units.begin(); it != units.end();) {
        if (!it->second->is_alive()) {
            destroy_unit(*it->second);
            it = units.erase(it);
        }
        else {
            ++it;
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

Map* Game::get_map() {
    return map.get();
}

void Game::init_player(int player_id, const vec2& starting_location) {
    auto player = std::make_shared<Player>(player_id, map->get_size());

    players.push_back(player);

    create_unit(UnitType::TownHall, starting_location, player_id);
}

void Game::create_unit(UnitType unit_type, const vec2& position, int player_id) {
    auto unit = std::make_unique<Unit>(unit_type, position, player_id);
    auto unit_id = unit->get_id();
    units.insert({ unit_id, std::move(unit) });
    auto moved_unit = units[unit_id].get();
    map->get_tile(position)->set_unit(*moved_unit);
    auto& player = players[player_id];

    player->modify_max_supply(Constants::unit_supply_provided.at(unit_type));
    player->modify_vision(*moved_unit, *map, 1);
}

void Game::destroy_unit(const Unit& unit) {
    auto unit_type = unit.get_type();
    auto player_id = unit.get_owner();
    auto& player = players[player_id];

    player->modify_supply(-Constants::unit_supply_cost.at(unit_type));
    player->modify_max_supply(-Constants::unit_supply_provided.at(unit_type));
    player->modify_vision(unit, *map, -1);

    map->get_tile(unit.get_position())->unset_unit();
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

bool Game::move_unit(Unit& unit, const vec2& target) {
    auto tile = map->get_tile(target);
    if (!tile->is_pathable()) {
        return false;
    }

    auto player_id = unit.get_owner();
    auto& player = players[player_id];

    // remove vision from the old location
    player->modify_vision(unit, *map, -1);

    // move the unit
    auto& old_position = unit.get_position();
    map->get_tile(old_position)->unset_unit();
    unit.set_position(target);
    tile->set_unit(unit);

    // add vision to the new location
    player->modify_vision(unit, *map, 1);

    return true;
}

void Game::enqueue_unit_action(std::shared_ptr<Unit> unit, std::unique_ptr<Action> action, bool replace_current_action) {
    unit->enqueue_action(std::move(action), replace_current_action);
    acting_units.insert(unit);
}