#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/ActionResult.h>
#include <Game/Actions/Move.h>
#include <Game/Player.h>
#include <Game/Unit.h>
#include <Game/UnitType.h>
#include <TypeDefs.h>
#include <memory>

class Build : public Action {
public:
    Build(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game, UnitType unit_type);
    ActionResult act() override;
    void cancel() override;
private:
    std::unique_ptr<Move> move_action;
    int progress;
    int target_progress;
    bool started;
    UnitType unit_type;
};