#pragma once
#include <Game/TileType.h>
#include <Game/UnitType.h>
#include <Game/AbilityType.h>
#include <unordered_map>
#include <unordered_set>
#include <string>

// TODO: Move most of this stuff to a json file to prevent having to recompile the entire project on changes
namespace Constants {
    static const int INITIAL_TILE_MINERALS = 1000;
    static const int INITIAL_PLAYER_MINERALS = 100;

    static const std::unordered_map<UnitType, std::unordered_set<AbilityType>> unit_abilities = {
        { UnitType::Worker, {
            AbilityType::Move,
            AbilityType::Gather,
            AbilityType::BuildTownHall,
            AbilityType::BuildFarm,
            AbilityType::BuildBarracks
        } },
        { UnitType::RangedWarrior, { AbilityType::Move, AbilityType::Attack } },
        { UnitType::MeleeWarrior, { AbilityType::Move, AbilityType::Attack } },
        { UnitType::TownHall, { AbilityType::TrainWorker } },
        { UnitType::Barracks, { AbilityType::TrainRangedWarrior, AbilityType::TrainMeleeWarrior } },
        { UnitType::Farm, { } }
    };

    static const std::unordered_map<TileType, bool> tile_pathable = {
        { TileType::Grass, true },
        { TileType::Water, false },
        { TileType::Mine, true },
        { TileType::Wall, false },
        { TileType::None, false }
    };

    static const std::unordered_map<TileType, bool> tile_blocks_vision = {
        { TileType::Grass, false },
        { TileType::Water, false },
        { TileType::Mine, false },
        { TileType::Wall, true },
        { TileType::None, false }
    };

    static const std::unordered_map<TileType, const std::string> tile_sprite = {
        { TileType::Grass, "\x1b[42m" },
        { TileType::Water, "\x1b[44m" },
        { TileType::Mine, "\x1b[43m" },
        { TileType::Wall, "\x1b[47m" },
        { TileType::None, "\x1b[m!" }
    };

    static const std::unordered_map<char, TileType> tile_letter = {
        { 'O', TileType::Grass },
        { ' ', TileType::Grass },
        { '~', TileType::Water },
        { 'X', TileType::Mine },
        { 'W', TileType::Wall },
        { '!', TileType::None }
    };

    static const std::unordered_map<UnitType, int> unit_mineral_cost = {
        { UnitType::Worker, 10 },
        { UnitType::RangedWarrior, 15 },
        { UnitType::MeleeWarrior, 10 },
        { UnitType::TownHall, 100 },
        { UnitType::Barracks, 50},
        { UnitType::Farm, 20 }
    };

    static const std::unordered_map<UnitType, char> unit_sprite = {
        { UnitType::Worker, 'w' },
        { UnitType::RangedWarrior, 'r' },
        { UnitType::MeleeWarrior, 'm' },
        { UnitType::TownHall, 'T' },
        { UnitType::Barracks, 'B' },
        { UnitType::Farm, 'F' }
    };

    static const std::unordered_map<UnitType, char> unit_create_sprite = {
        { UnitType::Worker, 'w' },
        { UnitType::RangedWarrior, 'r' },
        { UnitType::MeleeWarrior, 'm' },
        { UnitType::TownHall, 't' },
        { UnitType::Barracks, 'b' },
        { UnitType::Farm, 'f' }
    };

    static const std::unordered_map<UnitType, int> unit_health = {
        { UnitType::Worker, 10 },
        { UnitType::RangedWarrior, 10 },
        { UnitType::MeleeWarrior, 10 },
        { UnitType::TownHall, 300 },
        { UnitType::Barracks, 100 },
        { UnitType::Farm, 50 }
    };

    static const std::unordered_map<UnitType, int> unit_attack = {
        { UnitType::Worker, 1 },
        { UnitType::RangedWarrior, 3 },
        { UnitType::MeleeWarrior, 5 },
        { UnitType::TownHall, 0 },
        { UnitType::Barracks, 0 },
        { UnitType::Farm, 0 }
    };

    static const std::unordered_map<UnitType, int> unit_attack_range = {
        { UnitType::Worker, 1 },
        { UnitType::RangedWarrior, 3 },
        { UnitType::MeleeWarrior, 1 },
        { UnitType::TownHall, 0 },
        { UnitType::Barracks, 0 },
        { UnitType::Farm, 0 }
    };

    static const std::unordered_map<UnitType, int> unit_sight_range = {
        { UnitType::Worker, 5 },
        { UnitType::RangedWarrior, 5 },
        { UnitType::MeleeWarrior, 5 },
        { UnitType::TownHall, 7 },
        { UnitType::Barracks, 6 },
        { UnitType::Farm, 5 }
    };

    static const std::unordered_map<UnitType, int> unit_move_cooldown = {
        { UnitType::Worker, 2 },
        { UnitType::RangedWarrior, 1 },
        { UnitType::MeleeWarrior, 2 },
        { UnitType::TownHall, 0 },
        { UnitType::Barracks, 0 },
        { UnitType::Farm, 0 }
    };

    static const std::unordered_map<UnitType, int> unit_attack_cooldown = {
        { UnitType::Worker, 3 },
        { UnitType::RangedWarrior, 2 },
        { UnitType::MeleeWarrior, 3 },
        { UnitType::TownHall, 0 },
        { UnitType::Barracks, 0 },
        { UnitType::Farm, 0 }
    };

    static const std::unordered_map<UnitType, int> unit_create_time = {
        { UnitType::Worker, 5 },
        { UnitType::RangedWarrior, 10 },
        { UnitType::MeleeWarrior, 10 },
        { UnitType::TownHall, 100 },
        { UnitType::Barracks, 75 },
        { UnitType::Farm, 25 }
    };

    static const std::unordered_map<UnitType, int> unit_supply_cost = {
        { UnitType::Worker, 1 },
        { UnitType::RangedWarrior, 2 },
        { UnitType::MeleeWarrior, 2 },
        { UnitType::TownHall, 0 },
        { UnitType::Barracks, 0 },
        { UnitType::Farm, 0 }
    };

    static const std::unordered_map<UnitType, int> unit_supply_provided = {
        { UnitType::Worker, 0 },
        { UnitType::RangedWarrior, 0 },
        { UnitType::MeleeWarrior, 0 },
        { UnitType::TownHall, 10 },
        { UnitType::Barracks, 0 },
        { UnitType::Farm, 5 }
    };
};
