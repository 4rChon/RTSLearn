#include <Game/Actions/Build.h>
#include <Game/Constants.h>

Build::Build(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game, UnitType unit_type)
    : Action(target, actor, player, game, Constants::unit_create_sprite.at(unit_type))
    , unit_type(unit_type)
    , started(false)
    , progress(0)
    , target_progress(Constants::unit_create_time.at(unit_type)) {
}

ActionResult Build::act() {
    auto unit = actor.lock();
    if (!unit) {
        return ActionResult::Failure;
    }

    auto player = this->player.lock();
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

    if (unit->get_position() != target) {
        if (!move_action) {
            move_action = std::make_unique<Move>(target, actor, player, game);
        }

        auto move_result = move_action->act();
        if (move_result != ActionResult::Success) {
            return move_result;
        }
    }

    progress++;
    if (progress < target_progress) {
        return ActionResult::Running;
    }

    auto game = this->game.lock();
    if (!game) {
        return ActionResult::Failure;
    }

    auto nearest_pathable_tile = game->get_nearest_pathable_tile(target).lock();
    if (!nearest_pathable_tile) {
        return ActionResult::Running;
    }
    game->move_unit(unit, nearest_pathable_tile->get_position());
    game->create_unit(unit_type, target, player->get_id());

    return ActionResult::Success;
}

void Build::cancel() {
    if (move_action) {
        move_action->cancel();
    }

    if (started) {
        auto player = this->player.lock();
        player->modify_minerals(Constants::unit_mineral_cost.at(unit_type));
    }

    progress = 0;
}
