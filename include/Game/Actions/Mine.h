#pragma once
#include <Game/Actions/Action.h>

class Mine : public Action {
public:
    Mine(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game);
    ActionResult act() override;
    void cancel() override;
};