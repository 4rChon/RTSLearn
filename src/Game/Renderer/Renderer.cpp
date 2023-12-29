#include <Game/Renderer/Renderer.h>
#include <Game/Map.h>
#include <Game/Player.h>
#include <sstream>
#include <iostream>

namespace Renderer {
    void init(unsigned short max_fps) {
        max_delta_time = std::chrono::nanoseconds(1000000000 / max_fps);
        // resize the terminal to 80x100
        std::cout << "\x1b[8;150;150t\x1b[?25l";
    }

    void render(int ticks, std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Map> map) {
        auto start = std::chrono::high_resolution_clock::now();

        auto& size = map->get_size();
        auto x_max = size.second;
        auto y_max = size.first;

        buffers[buffer_index].append_line("\x1b[m\x1b[2J");

        for (auto y = 0; y < y_max; ++y) {
            for (auto x = 0; x < x_max; ++x) {
                if (players[0]->get_vision({ x, y })) {
                    auto tile = map->get_tile({ x, y });
                    buffers[buffer_index].append_line(tile->get_sprite());
                } else {
                    buffers[buffer_index].append_line("\x1b[40m  ");
                }
            }

            buffers[buffer_index].append_line("\x1b[m");
            buffers[buffer_index].new_line();
        }


        for (const auto& player : players) {
            buffers[buffer_index].append_line(player->get_resources_string());
            buffers[buffer_index].new_line();
        }

        buffers[buffer_index].append_line("Ticks: " + std::to_string(ticks));
        buffers[buffer_index].new_line();

        if (buffers[!buffer_index].empty()) {
            std::cout << buffers[buffer_index].build().str();
        } else {
            buffers[!buffer_index].reset_pointer();
            buffers[buffer_index].reset_pointer();

            std::stringstream delta_stream;
            while (buffers[!buffer_index].increment_pointer() && buffers[buffer_index].increment_pointer()) {
                auto str_1 = buffers[buffer_index].read();
                auto str_2 = buffers[!buffer_index].read();
                if (str_1 != str_2) {
                    auto cursor_position = buffers[!buffer_index].get_cursor_position();
                    delta_stream << "\x1b[" << cursor_position << str_1;
                }
            }

            std::cout << delta_stream.str();
        }

        buffers[!buffer_index] = buffers[buffer_index];
        buffers[buffer_index].clear();

        auto delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        while (delta_time < max_delta_time) {
            delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        }
    }

}