#include <Game/Actions/Move.h>
#include <Game/Pathfinder.h>

bool Move::can_act(const Unit* selected_unit, const Tile* target_tile) {
    return selected_unit
        && Constants::unit_abilities.at(selected_unit->get_type()).contains(AbilityType::Move)
        && target_tile
        && target_tile->is_pathable();
}

Move::Move(const vec2& target, Unit& actor, Player& player, Game& game)
    : Action(target, actor, player, game, 'm')
    , move_cooldown(0)
    , path_index(0) { }

ActionResult Move::act() {
    if (!actor) {
        return ActionResult::Failure;
    }

    if (actor->get_position() == target) {
        return ActionResult::Success;
    }

    ++move_cooldown;
    if (move_cooldown < Constants::unit_move_cooldown.at(actor->get_type())) {
        return ActionResult::Running;
    } else {
        move_cooldown = 0;
    }

    if (!game) {
        return ActionResult::Failure;
    }

    auto map = game->get_map();
    if (!map) {
        return ActionResult::Failure;
    }

    if (path_index == 0) {
        auto pathable_map = map->get_pathable_map();
        Pathfinder::get_path(actor->get_position(), target, pathable_map, map->get_size(), path);
    }

    if (path.size() == 0) {
        return ActionResult::Failure;
    }

    if (!game->move_unit(*actor, path[path.size() - (2 + path_index++)])) {
        path_index = 0;
    }

    return ActionResult::Running;
}

void Move::cancel() {
    path_index = 0;
    path.clear();
}
