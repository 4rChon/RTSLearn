#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/Move.h>
#include <memory>

class Gather : public Action {
public:
    Gather(const vec2& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game);
    ActionResult act() override;
    void cancel() override;
private:
    std::unique_ptr<Move> move_action;
    int progress;
    int target_progress;
};