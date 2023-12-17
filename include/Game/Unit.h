#pragma once
#include <Game/Actions/Action.h>
#include <Game/Constants.h>
#include <Game/UnitType.h>
#include <TypeDefs.h>
#include <string>
#include <queue>
#include <memory>
#include <mutex>

class Action;

class Unit {
public:
    Unit(UnitType type, const Vec2i& position, int owner);

    void set_position(const Vec2i& position);

    int get_id() const;
    int get_owner() const;
    const Vec2i& get_position() const;
    UnitType get_type() const;
    const std::string get_sprite();

    void enqueue_action(std::unique_ptr<Action> player_input, bool replace_current_action);
    void act();
    std::vector<ActionType> get_available_actions() const;
private:
    std::string sprite;
    static int next_id;
    const int id;
    const int owner;
    const UnitType type;
    Vec2i position;
    int health;
    std::queue<std::unique_ptr<Action>> action_queue;
    // keep a lock resource for the action queue so the renderer doesn't try to render the unit while it's being modified
    std::mutex action_queue_lock;
};