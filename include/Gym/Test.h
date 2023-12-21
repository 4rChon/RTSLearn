#pragma once
#include <Game/ActionType.h>
#include <Game/PlayerInput.h>
#include <vector>

namespace Gym {
    std::vector<PlayerInput> sample_noops(int player_id, int max_steps) {
        std::vector<PlayerInput> actions;
        for (int i = 0; i < max_steps; ++i) {
            actions.push_back({ player_id, ActionType::Noop, { 1, 1 } });
        }
        return actions;
    }

    const PlayerInput noop_action_0 = { 1, ActionType::Noop, { 1, 1 } };
    const PlayerInput noop_action_1 = { 1, ActionType::Noop, { 1, 1 } };
}