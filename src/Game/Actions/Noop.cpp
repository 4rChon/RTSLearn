#include <Game/Actions/Noop.h>
#include <Game/Actions/ActionResult.h>

Noop::Noop(const vec2& target, Unit& actor, Player& player, Game& game)
    : Action(target, actor, player, game, '.') { }

ActionResult Noop::act() { return ActionResult::Success; }

void Noop::cancel() { /* Do nothing */ }
