#pragma once
#include <Game/Actions/ActionResult.h>
#include <Game/Game.h>
#include <Game/Player.h>
#include <Game/Unit.h>
#include <TypeDefs.h>
#include <memory>

class Unit;
class Player;
class Game;

class Action {
public:
	Action(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game);
	virtual ActionResult act() = 0;
	virtual void cancel() = 0;
protected:
	const Vec2i target;
	std::weak_ptr<Unit> actor;
	std::weak_ptr<Player> player;
	std::weak_ptr<Game> game;
};