#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/ActionResult.h>
#include <Game/Game.h>
#include <Game/Player.h>
#include <Game/Unit.h>
#include <TypeDefs.h>

class Noop : Action {
public:
    Noop(const vec2& target, Unit& actor, Player& player, Game& game);
    ActionResult act() override;
    void cancel() override;
};