#pragma once
#include <Game/Player.h>
#include <Game/Map.h>
#include <string>
#include <memory>
#include <vector>

namespace Renderer {
    static std::string buffer[2];
    static bool buffer_index;
    static std::chrono::nanoseconds max_delta_time;
    void init(unsigned short max_fps);
    void render(int ticks, std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Map> map);
}