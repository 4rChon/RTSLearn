#include <Game/Game.h>
#include <Gym/Environment.h>
#include <Gym/Metadata.h>
#include <Gym/Observation.h>
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

    std::tuple<std::shared_ptr<Observation>, std::shared_ptr<Info>> Environment::reset(const std::string& map_name, unsigned int seed) {
        if (seed == 0) {
            this->seed = rd();
        }
        else {
            this->seed = seed;
        }

        gen = std::mt19937(this->seed);
        tick = 0;

        if (game != nullptr) {
            game.reset();
        }

        game = std::make_shared<Game>(metadata.render_fps);
        game->load_map(map_name);

        auto obs = _observation();
        auto info = _info();

        if (metadata.render_mode == RenderMode::Human) {
            render_thread = std::thread([&]() {
                while (!done) {
                    this->render();
                }
            });
        }

        auto return_val = std::tuple<std::shared_ptr<Observation>, std::shared_ptr<Info>>(obs, info);

        return return_val;
    }

    std::tuple<std::shared_ptr<Observation>, double, bool, bool, std::shared_ptr<Info>> Environment::step(std::vector<PlayerInput>& actions) {
        for (const PlayerInput& player_action : actions) {
            game->buffer_action(player_action);
        }

        game->step();

        ++tick;

        auto obs = _observation();
        double reward = 0.0;
        bool terminated = false;
        bool truncated = tick >= metadata.max_steps;
        auto info = _info();

        done = terminated || truncated;

        auto return_val = std::tuple<const std::shared_ptr<Observation>, double, bool, bool, const std::shared_ptr<Info>>(obs, reward, terminated, truncated, info);

        return return_val;
    }

    const std::shared_ptr<Info> Environment::_info() const {
        return std::make_shared<Info>();
    }

    const std::shared_ptr<Observation> Environment::_observation() const {
        return std::make_shared<Observation>();
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

    std::mt19937 Environment::get_generator() const {
        return gen;
    }

    const std::pair<double, double>& Environment::get_reward_range() const {
        return std::pair<double, double>(0.0, 1.0);
    }

    const PlayerInput Environment::sample_action(int player_id) {
        auto& map_size = game->get_map().lock()->get_size();

        auto available_actions = game->get_available_actions(player_id);
        std::uniform_int_distribution<> dis(0, available_actions.size()-1);
        ActionType action_type = available_actions[dis(gen)];

        std::uniform_int_distribution<> dis_x(0, map_size.first - 1);
        std::uniform_int_distribution<> dis_y(0, map_size.second - 1);
        Vec2i position = Vec2i(dis_x(gen), dis_y(gen));

        return PlayerInput{ player_id, action_type, position };
    }
}