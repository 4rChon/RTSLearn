#pragma once
#include <Game/Constants.h>
#include <Game/TileType.h>
#include <Game/Tile.h>
#include <sstream>

Tile::Tile(Vec2i position, TileType type, bool pathable, int minerals) 
    : position(position)
    , type(type)
    , pathable(pathable)
    , minerals(minerals) {}

TileType Tile::get_type() const {
    return type;
}

bool Tile::is_pathable(bool ignore_unit) const {
    if (!ignore_unit && !unit.expired()) {
        return false;
    }

    return pathable;
}

int Tile::get_minerals() const {
    return minerals;
}

const Vec2i& Tile::get_position() const {
    return position;
}

std::weak_ptr<Unit> Tile::get_unit() const {
    return unit;
}

std::string Tile::get_sprite() const {
    std::stringstream ss;
    ss << Constants::tile_sprite.at(type);
    if (auto unit_ptr = unit.lock()) {
        ss << unit_ptr->get_sprite();
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

void Tile::set_unit(std::weak_ptr<Unit> unit) {
    this->unit = unit;
}

void Tile::unset_unit() {
    this->unit.reset();
}
