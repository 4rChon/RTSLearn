#pragma once
#include <Game/Actions/ActionResult.h>
#include <Game/Game.h>
#include <Game/Player.h>
#include <Game/Unit.h>
#include <Game/ActionType.h>
#include <TypeDefs.h>
#include <memory>

class Unit;
class Player;
class Game;

class Action {
public:
    Action(const vec2& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game, char sprite);
    virtual ~Action() = default;
    virtual ActionResult act() = 0;
    virtual void cancel() = 0;
    char get_sprite() const;
protected:
    char sprite;
    const vec2 target;
    std::weak_ptr<Unit> actor;
    std::weak_ptr<Player> player;
    std::weak_ptr<Game> game;
};