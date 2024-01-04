#pragma once
#include <Game/Actions/ActionResult.h>
#include <Game/Game.h>
#include <Game/Player.h>
#include <Game/Unit.h>
#include <TypeDefs.h>

class Unit;
class Player;
class Game;

class Action {
public:
    Action(const vec2& target, Unit& actor, Player& player, Game& game, char sprite);
    virtual ~Action() = default;
    virtual ActionResult act() = 0;
    virtual void cancel() = 0;
    char get_sprite() const;
protected:
    const vec2 target;
    char sprite;
    Unit* actor;
    Player* player;
    Game* game;
};