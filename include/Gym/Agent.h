#pragma once
#include <Gym/Observation.h>
#include <Game/ActionType.h>
#include <Game/PlayerInput.h>
#include <TypeDefs.h>
#include <utility>
#include <memory>

namespace Gym {
	class Agent {
	public:
		const PlayerInput& get_action(std::weak_ptr<Observation> obs) const;
	};
}
