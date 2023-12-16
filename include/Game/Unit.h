#pragma once
#include <Game/Actions/Action.h>
#include <Game/Constants.h>
#include <Game/UnitType.h>
#include <TypeDefs.h>
#include <string>
#include <queue>
#include <memory>

class Action;

class Unit {
public:
    Unit(UnitType type, const Vec2i& position, int owner);

    void set_position(const Vec2i& position);

    int get_id() const;
    int get_owner() const;
    const Vec2i& get_position() const;
    UnitType get_type() const;
    const std::string get_sprite() const;

    void enqueue_action(std::unique_ptr<Action> player_input, bool replace_current_action);
    void act();
    std::vector<ActionType> get_available_actions() const;
private:
    static int next_id;
    const int id;
    const int owner;
    const UnitType type;
    Vec2i position;
    int health;
    std::queue<std::unique_ptr<Action>> action_queue;
};