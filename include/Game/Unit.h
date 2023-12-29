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
    Unit(UnitType type, const vec2& position, int owner);

    void set_position(const vec2& position);

    int get_id() const;
    int get_owner() const;
    const vec2& get_position() const;
    UnitType get_type() const;
    const std::string get_sprite();

    void enqueue_action(std::unique_ptr<Action> player_input, bool replace_current_action);
    ActionResult act();
    std::vector<ActionType> get_available_actions() const;
    bool modify_health(int amount);
    bool is_alive() const;
private:
    static int next_id;
    const int id;
    const int owner;
    const UnitType type;
    vec2 position;
    int health;
    std::queue<std::unique_ptr<Action>> action_queue;
    // keep a lock resource for the action queue so the renderer doesn't try to render the unit while it's being modified
    std::mutex action_queue_lock;
};