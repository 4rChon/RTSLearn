#include <Game/Actions/Attack.h>

bool Attack::can_act(const Unit* selected_unit, const Unit* target_unit, int player_id) {
    return selected_unit
        && target_unit
        && target_unit->get_owner() != player_id
        && Constants::unit_abilities.at(selected_unit->get_type()).contains(AbilityType::Attack);
}

Attack::Attack(const vec2& target, Unit& actor, Player& player, Game& game)
    : Action(target, actor, player, game, 'a')
    , attack_cooldown(0) {
    target_unit = game.get_map()->get_tile(target)->get_unit();
}

ActionResult Attack::act() {
    if (!actor) {
        return ActionResult::Failure;
    }

    if (!target_unit || !target_unit->is_alive()) {
        return ActionResult::Success;
    }

    auto range = Constants::unit_attack_range.at(actor->get_type());
    auto& position = actor->get_position();
    auto& target_position = target_unit->get_position();
    auto man_dist = std::abs(target_position.first - position.first) + std::abs(target_position.second - position.second);

    auto map = game->get_map();
    if (!map) {
        return ActionResult::Failure;
    }

    if (man_dist > range || !map->has_line_of_sight(position, target_position)) {
        if (!move_action) {
            move_action = std::make_unique<Move>(target_position, *actor, *player, *game);
        }
        auto move_result = move_action->act();
        if (move_result != ActionResult::Success) {
            return move_result;
        }
    }

    ++attack_cooldown;
    if (attack_cooldown < Constants::unit_attack_cooldown.at(actor->get_type())) {
        return ActionResult::Running;
    } else {
        attack_cooldown = 0;
    }

    auto is_alive = target_unit->modify_health(-Constants::unit_attack.at(actor->get_type()));
    if (!is_alive) {
        return ActionResult::Success;
    }

    return ActionResult::Running;
}

void Attack::cancel() {
    if (move_action) {
        move_action->cancel();
    }

    target_unit = nullptr;
}
