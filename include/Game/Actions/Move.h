#pragma once
#include <Game/Actions/Action.h>

class Move : public Action {
public:
    Move(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game);
    ActionResult act() override;
    void cancel() override;
    char get_action_sprite() const override;
private:
    int path_index = 0;
    int move_cooldown;
    std::vector<Vec2i> path;
};