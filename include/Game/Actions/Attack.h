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
    Attack(const vec2& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game);
    ActionResult act() override;
    void cancel() override;
private:
    int attack_cooldown;
    std::weak_ptr<Unit> target_unit;
    std::unique_ptr<Move> move_action;
};