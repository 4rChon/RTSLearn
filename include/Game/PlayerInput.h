#pragma once
#include <Game/ActionType.h>
#include <TypeDefs.h>

struct PlayerInput {
	const int player_id;
	const ActionType type;
	const Vec2i target;
};