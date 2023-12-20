#pragma once
#include <Game/Constants.h>
#include <Game/TileType.h>
#include <Game/Unit.h>
#include <TypeDefs.h>
#include <string>
#include <memory>

class Unit;

class Tile {
public:
    Tile(vec2 position, TileType type);

    TileType get_type() const;
    bool is_pathable(bool ignore_unit = false) const;
    bool is_vision_blocker() const;
    int get_minerals() const;
    const vec2& get_position() const;
    std::weak_ptr<Unit> get_unit() const;
    std::string get_sprite() const;

    void set_minerals(int minerals);
    void set_pathable(bool pathable);
    void set_type(TileType type);
    void set_unit(std::weak_ptr<Unit> unit);
    void unset_unit();

private:
    TileType type;
    bool pathable;
    bool blocks_vision;
    int minerals;
    const vec2 position;
    std::weak_ptr<Unit> unit;
};