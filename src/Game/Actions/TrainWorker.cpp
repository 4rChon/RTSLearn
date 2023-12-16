#include <Game/Actions/TrainWorker.h>
#include <Game/Constants.h>

TrainWorker::TrainWorker(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game)
	: Action(target, actor, player, game)
	, progress(0) {
	target_progress = Constants::unit_create_time.at(UnitType::Worker);
}

ActionResult TrainWorker::act() {
	if (progress != 0 && progress < target_progress) {
		progress++;

		return ActionResult::Running;
	}

	if (progress == target_progress) {
		auto game = this->game.lock();
		if (!game) {
			return ActionResult::Failure;
		}

		auto& position = this->actor.lock()->get_position();
		auto tile = game->get_nearest_pathable_tile(position).lock();
		
		if (!tile) {
			return ActionResult::Running;
		}

		auto player = this->player.lock();
		game->create_unit(UnitType::Worker, tile->get_position(), player->get_id());

		progress = 0;

		return ActionResult::Success;
	}

	auto player = this->player.lock();
	if (!player) {
		return ActionResult::Failure;
	}

	auto minerals = player->get_minerals();
	auto free_supply = player->get_free_supply();
	auto mineral_cost = Constants::unit_mineral_cost.at(UnitType::Worker);
	auto supply_cost = Constants::unit_supply_cost.at(UnitType::Worker);

	if (mineral_cost > minerals) {
		return ActionResult::Failure;
	}

	if (supply_cost > free_supply) {
		return ActionResult::Failure;
	}

	player->modify_minerals(-mineral_cost);
	player->modify_supply(supply_cost);

	progress++;

	return ActionResult::Running;
}

void TrainWorker::cancel() {
	auto player = this->player.lock();
	player->modify_minerals(Constants::unit_mineral_cost.at(UnitType::Worker));
	player->modify_supply(-Constants::unit_supply_cost.at(UnitType::Worker));

	progress = 0;
}
