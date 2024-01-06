#pragma once
#include <Game/Actions/ActionType.h>
#include <Game/Game.h>
#include <Game/PlayerInput.h>
#include <Gym/Metadata.h>
#include <Gym/Observation.h>
#include <Gym/Info.h>
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
        std::tuple<Observation*, Info*> reset(const std::string& map_name, unsigned int seed = 0);
        std::tuple<Observation*, double, bool, bool, Info*> step(const std::vector<PlayerInput>& action);
        void close();
        void render() const;
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

        Info* _info() const;
        Observation* _observation() const;
    };
}