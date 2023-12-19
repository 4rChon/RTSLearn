#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace ResourceLoader {
    const nlohmann::json load_json(const std::string& filename);
}
