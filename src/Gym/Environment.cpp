#include <Game/Game.h>
#include <Gym/Environment.h>
#include <Gym/Metadata.h>
#include <Gym/Observation.h>
#include <Gym/Info.h>
#include <TypeDefs.h>
#include <memory>
#include <random>
#include <string>
#include <utility>

namespace Gym {
    Environment::Environment(const Metadata& metadata)
        : metadata(metadata)
        , seed(0)
        , tick(0)
        , done(false) {
    }

    std::tuple<Observation*, Info*> Environment::reset(const std::string& map_name, unsigned int seed) {
        if (seed == 0) {
            this->seed = rd();
        } else {
            this->seed = seed;
        }

        done = false;
        gen = std::mt19937(this->seed);
        tick = 0;

        if (game != nullptr) {
            game.reset();
        }

        game = std::make_shared<Game>(metadata.render_fps);
        game->load_map(map_name);

        if (metadata.render_mode == RenderMode::Human) {
            render_thread = std::thread([&]() {
                while (!done) {
                    this->render();
                }
            });
        }

        return { _observation(), _info() };
    }

    std::tuple<Observation*, double, bool, bool, Info*> Environment::step(const std::vector<PlayerInput>& actions) {
        for (const PlayerInput& player_action : actions) {
            game->buffer_action(player_action);
        }

        game->step();

        ++tick;

        bool terminated = false;
        bool truncated = tick >= metadata.max_steps;
        
        done = terminated || truncated;

        return { _observation(), 0.0, terminated, truncated, _info() };
    }

    Info* Environment::_info() const {
        return new Info();
    }

    Observation* Environment::_observation() const {
        return new Observation();
    }

    void Environment::close() {
        done = true;
        if (render_thread.joinable()) {
            render_thread.join();
        }
    }

    void Environment::render() const {
        game->render();
    }

    const std::pair<double, double>& Environment::get_reward_range() const {
        return std::pair<double, double>(0.0, 1.0);
    }

    const PlayerInput Environment::sample_action(int player_id) {
        auto& map_size = game->get_map()->get_size();

        auto available_actions = game->get_available_actions(player_id);
        std::uniform_int_distribution<> dis(0, available_actions.size()-1);
        ActionType action_type = available_actions[dis(gen)];

        std::uniform_int_distribution<> dis_x(0, map_size.first - 1);
        std::uniform_int_distribution<> dis_y(0, map_size.second - 1);
        vec2 position = vec2(dis_x(gen), dis_y(gen));

        return PlayerInput{ player_id, action_type, position };
    }
}