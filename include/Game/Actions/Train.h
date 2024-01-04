#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/ActionResult.h>
#include <Game/Game.h>
#include <Game/Player.h>
#include <Game/Unit.h>
#include <Game/UnitType.h>
#include <TypeDefs.h>

class Train : public Action {
public:
    static bool can_act(const Unit* selected_unit, ActionType action_type);

    Train(const vec2& target, Unit& actor, Player& player, Game& game, UnitType unit_type);
    ActionResult act() override;
    void cancel() override;
private:
    int progress;
    int target_progress;
    UnitType unit_type;
};