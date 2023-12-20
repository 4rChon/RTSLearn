#include <Game/Actions/Gather.h>
#include <Game/Actions/Move.h>

Gather::Gather(const vec2& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game)
    : Action(target, actor, player, game, 'g')
    , target_progress(5)
    , progress(0) {
}

ActionResult Gather::act() {
    auto unit = actor.lock();
    if (!unit) {
        return ActionResult::Failure;
    }

    auto tile = game.lock()->get_map().lock()->get_tile(target).lock();
    if (tile->get_type() != TileType::Mine || tile->get_minerals() <= 0) {
        return ActionResult::Failure;
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

    ++progress;
    if (progress < target_progress) {
        return ActionResult::Running;
    }

    if (auto player = this->player.lock()) {
        player->modify_minerals(10);
        progress = 0;
        tile->set_minerals(tile->get_minerals() - 10);

        unit->enqueue_action(std::make_unique<Gather>(target, actor, player, game), false);
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


