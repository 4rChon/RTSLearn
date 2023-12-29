#include <Game/Actions/Build.h>
#include <Game/Constants.h>

bool Build::can_act(const Unit* selected_unit, ActionType action_type) {
    return selected_unit
        && Constants::unit_abilities.at(selected_unit->get_type()).contains((AbilityType)action_type);
}

Build::Build(const vec2& target, Unit& actor, Player& player, Game& game, UnitType unit_type)
    : Action(target, actor, player, game, Constants::unit_create_sprite.at(unit_type))
    , unit_type(unit_type)
    , started(false)
    , progress(0)
    , target_progress(Constants::unit_create_time.at(unit_type)) {
}

ActionResult Build::act() {
    if (!actor) {
        return ActionResult::Failure;
    }

    if (!player) {
        return ActionResult::Failure;
    }

    if (!started) {
        auto minerals = player->get_minerals();
        auto mineral_cost = Constants::unit_mineral_cost.at(unit_type);

        if (mineral_cost > minerals) {
            return ActionResult::Failure;
        }

        started = true;
        player->modify_minerals(-mineral_cost);
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

    if (!game) {
        return ActionResult::Failure;
    }

    auto nearest_pathable_tile = game->get_map()->get_nearest_pathable_tile(target);
    if (!nearest_pathable_tile) {
        return ActionResult::Running;
    }
    game->move_unit(*actor, nearest_pathable_tile->get_position());
    game->create_unit(unit_type, target, player->get_id());

    return ActionResult::Success;
}

void Build::cancel() {
    if (move_action) {
        move_action->cancel();
    }

    if (started) {
        player->modify_minerals(Constants::unit_mineral_cost.at(unit_type));
    }

    progress = 0;
}
