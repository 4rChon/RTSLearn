#include <Game/Actions/Gather.h>
#include <Game/Actions/Move.h>

Gather::Gather(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game)
    : Action(target, actor, player, game)
    , gather_max_progress(5)
    , gather_progress(0) {
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

    gather_progress++;
    if (gather_progress < gather_max_progress) {
        return ActionResult::Running;
    }

    if (auto player = this->player.lock()) {
        player->modify_minerals(10);
        gather_progress = 0;
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
    gather_progress = 0;
}

char Gather::get_action_sprite() const {
    return 'g';
}

