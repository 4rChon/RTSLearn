#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/Move.h>
#include <memory>

class Gather : public Action {
public:
    Gather(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game);
    ActionResult act() override;
    void cancel() override;
    char get_action_sprite() const override;
private:
    std::unique_ptr<Move> move_action;
    int gather_max_progress;
    int gather_progress;
};