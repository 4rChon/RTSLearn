#include <Game/Actions/Attack.h>

Attack::Attack(const vec2& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game)
    : Action(target, actor, player, game, 'a')
    , attack_cooldown(0) {
    target_unit = game.lock()->get_map().lock()->get_tile(target).lock()->get_unit();
}

ActionResult Attack::act() {
    auto unit = actor.lock();
    if (!unit) {
        return ActionResult::Failure;
    }

    auto target_unit = this->target_unit.lock();
    if (!target_unit || !target_unit->is_alive()) {
        return ActionResult::Success;
    }

    auto range = Constants::unit_attack_range.at(unit->get_type());
    auto& position = unit->get_position();
    auto& target_position = target_unit->get_position();
    auto man_dist = std::abs(target_position.first - position.first) + std::abs(target_position.second - position.second);

    auto map = game.lock()->get_map().lock();
    if (!map) {
        return ActionResult::Failure;
    }

    if (man_dist > range || !map->has_line_of_sight(position, target_position)) {
        if (!move_action) {
            move_action = std::make_unique<Move>(target_position, actor, player, game);
        }
        auto move_result = move_action->act();
        if (move_result != ActionResult::Success) {
            return move_result;
        }
    }

    ++attack_cooldown;
    if (attack_cooldown < Constants::unit_attack_cooldown.at(unit->get_type())) {
        return ActionResult::Running;
    } else {
        attack_cooldown = 0;
    }

    auto is_alive = target_unit->modify_health(-Constants::unit_attack.at(unit->get_type()));
    if (!is_alive) {
        return ActionResult::Success;
    }

    return ActionResult::Running;
}

void Attack::cancel() {
    if (move_action) {
        move_action->cancel();
    }

    target_unit.reset();
}
