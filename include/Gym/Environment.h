#pragma once
#include <Game/ActionType.h>
#include <Game/Game.h>
#include <Game/PlayerInput.h>
#include <Gym/Metadata.h>
#include <Gym/Observation.h>
#include <TypeDefs.h>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <thread>

namespace Gym {
	class Environment {
	public:
		Environment(const Metadata& metadata);
		std::tuple<std::shared_ptr<Observation>, std::shared_ptr<Info>> reset(const std::string& map_name, unsigned int seed = 0);
		std::tuple<std::shared_ptr<Observation>, double, bool, bool, std::shared_ptr<Info>> step(std::vector<PlayerInput>& action);
		void close();
		void render() const;
		std::mt19937 get_generator() const;
		const std::pair<double, double>& get_reward_range() const;
		const PlayerInput sample_action(int player_id);
	private:
		int tick;
		int seed;
		std::random_device rd;
		std::mt19937 gen;
		const Metadata metadata;
		std::shared_ptr<Game> game;
		std::thread render_thread;
		bool done;
		bool render_flag;

		const std::shared_ptr<Info> _info() const;
		const std::shared_ptr<Observation> _observation() const;
	};
}