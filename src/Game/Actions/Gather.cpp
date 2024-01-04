#include <Game/Actions/Gather.h>
#include <Game/Constants.h>

bool Gather::can_act(const Unit* selected_unit, const Tile* target_tile) {
    return selected_unit
        && Constants::unit_abilities.at(selected_unit->get_type()).contains(AbilityType::Gather)
        && target_tile
        && target_tile->get_type() == TileType::Mine;
}

Gather::Gather(const vec2& target, Unit& actor, Player& player, Game& game)
    : Action(target, actor, player, game, 'g')
    , target_progress(5)
    , progress(0) {
}

ActionResult Gather::act() {
    if (!actor) {
        return ActionResult::Failure;
    }

    auto tile = game->get_map()->get_tile(target);
    if (tile->get_type() != TileType::Mine || tile->get_minerals() <= 0) {
        return ActionResult::Failure;
    }

    if (actor->get_position() != target) {
        if (!move_action) {
            move_action = std::make_unique<Move>(target, *actor, *player, *game);
        }

        auto move_result = move_action->act();
        if (move_result != ActionResult::Success) {
            return move_result;
        }
    }

    ++progress;
    if (progress < target_progress) {
        return ActionResult::Running;
    }

    if (player) {
        player->modify_minerals(10);
        progress = 0;
        tile->set_minerals(tile->get_minerals() - 10);

        actor->enqueue_action(std::make_unique<Gather>(target, *actor, *player, *game), false);
        return ActionResult::Success;
    }

    return ActionResult::Failure;
}

void Gather::cancel() {
    if (move_action) {
        move_action->cancel();
    }
    progress = 0;
}


