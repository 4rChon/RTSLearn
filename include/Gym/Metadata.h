#pragma once

namespace Gym {
	enum class RenderMode {
		None = 0,
		Human
	};

	struct Metadata {
		RenderMode render_mode;
		int render_fps;
		int player_count;
		int max_steps;
	};
}