#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/Move.h>
#include <Game/Actions/ActionResult.h>
#include <Game/Player.h>
#include <Game/Game.h>
#include <TypeDefs.h>
#include <memory>

class Attack : public Action {
public:
    static bool can_act(const Unit* selected_unit, const Unit* target_unit, int player_id);
    Attack(const vec2& target, Unit& actor, Player& player, Game& game);
    ActionResult act() override;
    void cancel() override;
private:
    int attack_cooldown;
    Unit* target_unit;
    std::unique_ptr<Move> move_action;
};