#pragma once
#include <Game/Actions/Action.h>
#include <Game/Actions/ActionResult.h>

class TrainWorker : public Action {
public:
	TrainWorker(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game);
	ActionResult act() override;
	void cancel() override;
private:
	int progress;
	int target_progress;
};