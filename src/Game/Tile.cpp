#pragma once
#include <Game/Constants.h>
#include <Game/TileType.h>
#include <Game/Tile.h>
#include <sstream>

Tile::Tile(vec2 position, TileType type) 
    : position(position)
    , type(type)
    , pathable(Constants::tile_pathable.at(type))
    , blocks_vision(Constants::tile_blocks_vision.at(type))
    , minerals(type == TileType::Mine ? Constants::INITIAL_TILE_MINERALS : 0)
    , unit(nullptr) {}

TileType Tile::get_type() const {
    return type;
}

bool Tile::is_pathable(bool ignore_unit) const {
    return (ignore_unit || !unit) && pathable;
}

bool Tile::is_vision_blocker() const {
    return blocks_vision;
}

int Tile::get_minerals() const {
    return minerals;
}

const vec2& Tile::get_position() const {
    return position;
}

Unit* Tile::get_unit() const {
    return unit;
}

std::string Tile::get_sprite() const {
    std::stringstream ss;
    ss << Constants::tile_sprite.at(type);
    if (unit) {
        ss << unit->get_sprite();
    }
    else {
        ss << "  ";
    }

    return ss.str();
}

void Tile::set_minerals(int minerals) {
    this->minerals = minerals;

    if (this->minerals <= 0 && type == TileType::Mine) {
        type = TileType::Grass;
    } else if (this->minerals > 0 && type == TileType::Grass) {
        type = TileType::Mine;
    }
}

void Tile::set_pathable(bool pathable) {
    this->pathable = pathable;
}

void Tile::set_type(TileType type) {
    this->type = type;
}

void Tile::set_unit(Unit& unit) {
    this->unit = &unit;
}

void Tile::unset_unit() {
    this->unit = nullptr;
}
