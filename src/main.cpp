constexpr auto MAX_STEPS = 1000000;
constexpr auto MAX_EPISODES = 1;

#define SAVE_REPLAY 0
#define RUN_REPLAY 0

#include <Gym/Environment.h>
#include <Gym/Test.h>
#include <chrono>
#include <iostream>
#include <Game/ResourceLoader.h>

using namespace Gym;

#if RUN_REPLAY
static long long run_replay(Environment& env, const std::vector<std::vector<PlayerInput>>& actions) {
    auto [obs, info] = env.reset("assets/map.json");
    bool done = false;
    int i = 0;

    auto start_time = std::chrono::high_resolution_clock::now();
    while (!done) {
        auto [next_obs, reward, terminated, truncated, info] = env.step({ actions[0][i], actions[1][i] });
        done = terminated || truncated;

        delete info;
        delete obs;
        obs = next_obs;

        i++;

    }

    delete obs;

    auto end_time = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
}
#else
static long long run_episode(Environment& env) {
    auto [obs, info] = env.reset("assets/map.json");
    bool done = false;

#if SAVE_REPLAY
    std::vector<PlayerInput> p1_actions;
    std::vector<PlayerInput> p2_actions;
#endif

    auto start_time = std::chrono::high_resolution_clock::now();
    while (!done) {
        // Action action = agent.get_action(obs);
        std::vector<PlayerInput> actions = {
            env.sample_action(0),
            env.sample_action(1)
        };

#if SAVE_REPLAY
        p1_actions.push_back(actions[0]);
        p2_actions.push_back(actions[1]);
#endif

        auto [next_obs, reward, terminated, truncated, info] = env.step(actions);

        // agent.update(obs, action, reward, terminated, next_obs);

        done = terminated || truncated;

        delete info;
        delete obs;
        obs = next_obs;
    }

    delete obs;

    auto end_time = std::chrono::high_resolution_clock::now();
    auto run_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

#if SAVE_REPLAY
    ResourceLoader::save_replay("assets/replay.json", { p1_actions, p2_actions });
#endif

    return run_time;
}
#endif

int main() {
    Environment env({ RenderMode::Human, 16, 2, MAX_STEPS });
    bool done = false;

    auto run_time = 0ll;
    for (int episode = 0; episode < MAX_EPISODES; ++episode)
    {
#if RUN_REPLAY
        auto actions = ResourceLoader::load_replay("assets/replay.json");
        run_time += run_replay(env, actions);
#else
        run_time += run_episode(env);
#endif
        env.close();
    }

    std::cout << "Average Frame Time: " << (double)run_time / (double)(MAX_STEPS * MAX_EPISODES) << "ns" << std::endl;
    std::cout << "FPS: " << 1000000000 / ((double)run_time / (double)(MAX_STEPS * MAX_EPISODES)) << std::endl;
    std::cout << "Total time: " << run_time << "ns" << std::endl;

    return 0;
}