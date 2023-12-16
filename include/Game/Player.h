#pragma once
#include <memory>
#include <string>

class Player {
public:
    Player(int id);
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
private:
    const int id;
    int minerals;
    int max_supply;
    int supply;
    int selected_unit_id;
};