#include <Game/ResourceLoader.h>
#include <fstream>

namespace ResourceLoader {
    const nlohmann::json load_json(const std::string& filename) {
        std::ifstream file{ filename };

        return nlohmann::json::parse(file);
    }
}