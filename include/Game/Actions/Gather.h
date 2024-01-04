#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/Move.h>
#include <Game/Actions/ActionResult.h>
#include <Game/Unit.h>
#include <Game/Player.h>
#include <Game/Game.h>
#include <TypeDefs.h>
#include <memory>

class Gather : public Action {
public:
    static bool can_act(const Unit* selected_unit, const Tile* target_tile);

    Gather(const vec2& target, Unit& actor, Player& player, Game& game);
    ActionResult act() override;
    void cancel() override;
private:
    std::unique_ptr<Move> move_action;
    int progress;
    int target_progress;
};