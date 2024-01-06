#pragma once
#include <Game/Actions/ActionType.h>
#include <TypeDefs.h>

struct PlayerInput {
    const int player_id;
    const ActionType type;
    const vec2 target;
};