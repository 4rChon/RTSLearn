#include <Game/Actions/Noop.h>
#include <Game/Actions/ActionResult.h>

Noop::Noop(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game)
	: Action(target, actor, player, game) { }

ActionResult Noop::act() { return ActionResult::Success; }

void Noop::cancel() { /* Do nothing */ }
