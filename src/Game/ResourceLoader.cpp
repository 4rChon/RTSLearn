#include <Game/ResourceLoader.h>
#include <fstream>

namespace ResourceLoader {
    const nlohmann::json load_json(const std::string& filename) {
        std::ifstream file;
        file.open(filename);

        if (file.fail()) {
            perror(filename.c_str());

            return nlohmann::json();
        }

        try {
            return nlohmann::json::parse(file);
        }
        catch (nlohmann::json::exception& e) {
            file.close();
            perror(e.what());

            return nlohmann::json();
        }

        file.close();

        return nlohmann::json();
    }
}