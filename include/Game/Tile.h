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
    Tile(Vec2i position, TileType type, bool pathable, int minerals);

    TileType get_type() const;
    bool is_pathable(bool ignore_unit = false) const;
    int get_minerals() const;
    const Vec2i& get_position() const;
    std::weak_ptr<Unit> get_unit() const;
    std::string get_sprite() const;

    void set_minerals(int minerals);
    void set_pathable(bool pathable);
    void set_type(TileType type);
    void set_unit(std::weak_ptr<Unit> unit);
    void unset_unit();


    static std::shared_ptr<Tile> create_tile(const Vec2i& position, char letter) {
        switch (letter) {
            case ' ':
                return std::make_shared<Tile>(position, TileType::Grass, true, 0);
            case '~':
                return std::make_shared<Tile>(position, TileType::Water, false, 0);
            case 'X':
                return std::make_shared<Tile>(position, TileType::Mine, true, Constants::INITIAL_TILE_MINERALS);
            case 'O':
                return std::make_shared<Tile>(position, TileType::Grass, true, 0);
            default:
                return std::make_shared<Tile>(position, TileType::Grass, true, 0);
        }
    }
private:
    TileType type;
    bool pathable;
    int minerals;
    const Vec2i position;
    std::weak_ptr<Unit> unit;
};