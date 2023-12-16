#include <Game/Actions/Move.h>
#include <Game/Pathfinder.h>

Move::Move(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game)
    : Action(target, actor, player, game)
    , move_cooldown(0) {
}

ActionResult Move::act() {
    auto unit = actor.lock();
    if (!unit) {
        return ActionResult::Failure;
    }

    if (unit->get_position() == target) {
        return ActionResult::Success;
    }

    if (move_cooldown < Constants::unit_move_cooldown.at(unit->get_type())) {
        move_cooldown++;
        return ActionResult::Running;
    } else {
        move_cooldown = 0;
    }

    auto game = this->game.lock();
    if (!game) {
        return ActionResult::Failure;
    }

    auto map = game->get_map().lock();
    if (!map) {
        return ActionResult::Failure;
    }

    if (path_index == 0) {
        auto pathable_map = map->get_pathable_map();
        path = Pathfinder::get_path(unit->get_position(), target, pathable_map);
    }

    if (path.size() == 0) {
        return ActionResult::Failure;
    }

    if (!game->move_unit(unit, path[path.size() - (2 + path_index++)])) {
        path_index = 0;
    }

    return ActionResult::Running;
}

void Move::cancel() {
    path_index = 0;
    path = {};
}
