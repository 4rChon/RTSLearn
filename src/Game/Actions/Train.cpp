#include <Game/Actions/Train.h>
#include <Game/Constants.h>

Train::Train(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game, UnitType unit_type)
    : Action(target, actor, player, game, Constants::unit_create_sprite.at(unit_type))
    , unit_type(unit_type)
    , progress(0)
    , target_progress(Constants::unit_create_time.at(unit_type)) {
}

ActionResult Train::act() {
    auto unit = actor.lock();
    if (!unit) {
        return ActionResult::Failure;
    }

    auto player = this->player.lock();
    if (!player) {
        return ActionResult::Failure;
    }

    if (progress == 0) {
        auto minerals = player->get_minerals();
        auto free_supply = player->get_free_supply();
        auto mineral_cost = Constants::unit_mineral_cost.at(unit_type);
        auto supply_cost = Constants::unit_supply_cost.at(unit_type);

        if (mineral_cost > minerals) {
            return ActionResult::Failure;
        }

        if (supply_cost > free_supply) {
            return ActionResult::Failure;
        }

        player->modify_minerals(-mineral_cost);
        player->modify_supply(supply_cost);
    }

    ++progress;
    if (progress < target_progress) {
        return ActionResult::Running;
    }

    if (progress >= target_progress) {
        auto game = this->game.lock();
        if (!game) {
            return ActionResult::Failure;
        }

        auto& position = this->actor.lock()->get_position();
        auto nearest_pathable_tile = game->get_nearest_pathable_tile(position).lock();
        if (!nearest_pathable_tile) {
            return ActionResult::Running;
        }

        game->create_unit(unit_type, nearest_pathable_tile->get_position(), player->get_id());

        return ActionResult::Success;
    }


    return ActionResult::Running;
}

void Train::cancel() {
    auto player = this->player.lock();
    if (progress > 0) {
        player->modify_minerals(Constants::unit_mineral_cost.at(unit_type));
        player->modify_supply(-Constants::unit_supply_cost.at(unit_type));
    }

    progress = 0;
}
