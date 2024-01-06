#pragma once
#include <TypeDefs.h>
#include <string>
#include <vector>

class Unit;
class Map;

class Player {
public:
    Player(int id, const vec2& map_size);
    int get_id() const;
    int get_minerals() const;
    int get_free_supply() const;
    void modify_minerals(int amount);
    void modify_supply(int amount);
    void modify_max_supply(int amount);
    void select_unit(int unit_id);
    void deselect_unit();
    int get_selected_unit_id() const;
    const std::string get_resources_string() const;
    void modify_vision(const Unit& unit, const Map& map, int value);
    bool has_vision(const vec2& position) const;
private:
    const int id;
    int minerals;
    int max_supply;
    int supply;
    int selected_unit_id;
    std::vector<std::vector<int>> vision;
};