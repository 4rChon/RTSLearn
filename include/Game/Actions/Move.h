#pragma once
#include <Game/Actions/Action.h>

class Move : public Action {
public:
    static bool can_act(const Unit* selected_unit, const Tile* target_tile);
    Move(const vec2& target, Unit& actor, Player& player, Game& game);
    ActionResult act() override;
    void cancel() override;
private:
    int path_index = 0;
    int move_cooldown;
    std::vector<vec2> path;
};