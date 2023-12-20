#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/ActionResult.h>

class Train : public Action {
public:
    Train(const vec2& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game, UnitType unit_type);
    ActionResult act() override;
    void cancel() override;
private:
    int progress;
    int target_progress;
    UnitType unit_type;
};