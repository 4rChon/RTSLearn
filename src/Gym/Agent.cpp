#include <Game/Actions/ActionType.h>
#include <Game/PlayerInput.h>
#include <Gym/Agent.h>
#include <Gym/Observation.h>
#include <memory>

namespace Gym {
    const PlayerInput& Agent::get_action(const std::weak_ptr<Observation> obs) const {
        return PlayerInput{ 0, ActionType::Noop, vec2(0, 0) };
    }
}