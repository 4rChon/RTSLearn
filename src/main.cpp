constexpr auto MAX_STEPS = 1000000;
constexpr auto MAX_EPISODES = 1;

#include <Gym/Environment.h>
#include <chrono>
#include <iostream>
#include <Gym/Test.h>

using namespace Gym;

int main() {
    Environment env({ RenderMode::Human, 16, 2, MAX_STEPS });
    bool done = false;

    auto total_time = 0ll;
    for (int episode = 0; episode < MAX_EPISODES; ++episode)
    {
        auto [obs, info] = env.reset("assets/map.json");
        done = false;

        auto start_time = std::chrono::high_resolution_clock::now();
        while (!done) {
            // Action action = agent.get_action(obs);
            std::vector<PlayerInput> actions = {
                env.sample_action(0),
                env.sample_action(1)
            };

            auto [next_obs, reward, terminated, truncated, info] = env.step(actions);

            // agent.update(obs, action, reward, terminated, next_obs);

            done = terminated || truncated;
            
            delete info;
            delete obs;
            obs = next_obs;
        }

        delete obs;

        auto end_time = std::chrono::high_resolution_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        env.close();
    }

    std::cout << "Average Frame Time: " << (double)total_time / (double)(MAX_STEPS * MAX_EPISODES) << "ns" << std::endl;
    std::cout << "FPS: " << 1000000000 / ((double)total_time / (double)(MAX_STEPS * MAX_EPISODES)) << std::endl;
    std::cout << "Total time: " << total_time << "ns" << std::endl;

    return 0;
}