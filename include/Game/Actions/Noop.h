#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/ActionResult.h>

class Noop : Action {
public:
    Noop(const vec2& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game);
    ActionResult act() override;
    void cancel() override;
};