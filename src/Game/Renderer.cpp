#include <Game/Renderer.h>
#include <Game/Map.h>
#include <Game/Player.h>
#include <sstream>
#include <iostream>

namespace Renderer {
    void init(unsigned short max_fps) {
        max_delta_time = std::chrono::nanoseconds(1000000000 / max_fps);
    }

    void render(int ticks, std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Map> map) {
        auto start = std::chrono::high_resolution_clock::now();

        // render map
        std::stringstream ss;
        ss << "\x1b[m\x1b[2J";

        auto& size = map->get_size();
        auto x_max = size.second;
        auto y_max = size.first;
        for (auto y = 0; y < y_max; ++y) {
            for (auto x = 0; x < x_max; ++x) {
                if (players[0]->get_vision({ x, y })) {
                    if (auto tile_ptr = map->get_tile({ x, y }).lock()) {
                        ss << tile_ptr->get_sprite();
                    }
                }
                else {
                    ss << "\x1b[40m  ";
                }
            }

            ss << "\x1b[m\n";
        }

        for (const auto& player : players) {
            ss << player->get_resources_string() << '\n';
        }

        ss << "Ticks: " << ticks << '\n';

        // TODO:
        // compare buffer diffs
        // move pointer to each diff and print

        buffer[buffer_index] = ss.str();
        std::cout << buffer[buffer_index];
        buffer_index = !buffer_index;

        auto delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        while (delta_time < max_delta_time) {
            delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        }
    }

}