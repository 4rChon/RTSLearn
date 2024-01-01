#include <Game/ResourceLoader.h>
#include <fstream>
#include <TypeDefs.h>
#include <Game/PlayerInput.h>
#include <utility>
#include <iostream>

namespace ResourceLoader {
    const nlohmann::json load_json(const std::string& filename) {
        std::ifstream file{ filename };

        return nlohmann::json::parse(file);
    }

    void save_replay(const std::string& filename, const std::vector<std::vector<PlayerInput>>& actions) {
        std::cout << "Saving replay to " << filename << '\n';
        std::ofstream replay_file(filename);

        nlohmann::json replay;
        for (const auto& player_actions : actions) {
            nlohmann::json player_action_json;
            for (const auto& action : player_actions) {
                player_action_json.push_back({
                    { "player_id", action.player_id },
                    { "action_type", action.type },
                    { "target", { action.target.first, action.target.second } }
                });
            }

            replay["actions"].push_back(player_action_json);
        }

        replay_file << replay.dump(4);

        std::cout << "Saved replay to " << filename << '\n';
    }

    std::vector<std::vector<PlayerInput>> load_replay(const std::string& filename) {
        std::cout << "Loading replay from " << filename << '\n';
        std::ifstream replay_file(filename);

        nlohmann::json replay;
        replay_file >> replay;

        std::vector<std::vector<PlayerInput>> actions;
        for (auto& player_action : replay["actions"]) {
            std::vector<PlayerInput> player_actions;
            for (auto& action : player_action) {
                player_actions.push_back(PlayerInput {
                    action["player_id"],
                    action["action_type"],
                    action["target"]
                });
            }
            actions.push_back(player_actions);
        }

        std::cout << "Loaded replay from " << filename << '\n';

        return actions;
    }
}