constexpr auto MAX_STEPS = 1000000;
constexpr auto MAX_EPISODES = 10;

#include <Gym/Environment.h>
#include <chrono>
#include <iostream>

using namespace Gym;

int main() {
    Environment env({ RenderMode::None, 60, 2, MAX_STEPS });
    std::shared_ptr<Observation> obs;
    std::shared_ptr<Observation> next_obs;
    double reward;
    bool terminated;
    bool truncated;
    std::shared_ptr<Info> info;
    bool done = false;

    for (int episode = 0; episode < MAX_EPISODES; episode++)
    {

        std::tie(obs, info) = env.reset("assets/map.json");
        done = false;

        auto start_time = std::chrono::high_resolution_clock::now();
        while (!done) {
            // Action action = agent.get_action(obs);
            std::vector<PlayerInput> actions = {
                env.sample_action(0),
                env.sample_action(1)
            };

            std::tie(next_obs, reward, terminated, truncated, info) = env.step(actions);

            // agent.update(obs, action, reward, terminated, next_obs);

            done = terminated || truncated;
            obs = next_obs;
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        std::cout << "Average Frame Time: " << (double)total_time / (double)MAX_STEPS << " nanoseconds" << std::endl;
        std::cout << "FPS: " << 1000000000 / ((double)total_time / (double)MAX_STEPS) << std::endl;
        std::cout << "Total time: " << total_time << " nanoseconds" << std::endl;

        env.close();
    }

    return 0;
}