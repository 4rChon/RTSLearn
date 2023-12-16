#include <Game/Actions/Mine.h>

Mine::Mine(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game)
: Action(target, actor, player, game) {
}

ActionResult Mine::act() {
    auto unit = actor.lock();
    if (!unit) {
        return ActionResult::Failure;
    }

    auto map = game.lock()->get_map().lock();
    if (!map) {
        return ActionResult::Failure;
    }

    auto tile = map->get_tile(target).lock();
    if (!tile) {
        return ActionResult::Failure;
    }

    if (tile->get_type() != TileType::Mine) {
        return ActionResult::Failure;
    }

    // TODO: Work on this

    return ActionResult::Success;
}

void Mine::cancel() {
}


