#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <Game/PlayerInput.h>

namespace ResourceLoader {
    const nlohmann::json load_json(const std::string& filename);
    void save_replay(const std::string& filename, const std::vector<std::vector<PlayerInput>>& actions);
    std::vector<std::vector<PlayerInput>> load_replay(const std::string& filename);
}
