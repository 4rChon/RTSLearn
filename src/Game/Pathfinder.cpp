#include <Game/Pathfinder.h>
#include <TypeDefs.h>
#include <unordered_map>
#include <queue>

namespace Pathfinder {
    void Pathfinder::get_path(const vec2& start, const vec2& end, const std::vector<bool>& map, const vec2& map_size, std::vector<vec2>& out_path) {
        auto& [map_width, map_height] = map_size;

        std::priority_queue<std::pair<unsigned char, vec2>, std::vector<std::pair<unsigned char, vec2>>, std::greater<std::pair<unsigned char, vec2>>> open_pq;
        open_pq.push({ 0, start });

        auto open_set = std::vector<bool>(map_width * map_height);
        auto came_from = std::vector<vec2>(map_width * map_height, { -1, -1 });
        auto g_score = std::vector<unsigned short>(map_width * map_height, std::numeric_limits<unsigned short>::max());

        g_score[start.first + start.second * map_width] = 0;
        open_set[start.first + start.second * map_width] = true;

        while (!open_pq.empty()) {
            auto current = open_pq.top().second;
            if (current == end) {
                out_path.push_back(current);

                while (came_from[current.first + current.second * map_width] != vec2{ -1, -1 }) {
                    current = came_from[current.first + current.second * map_width];
                    out_path.push_back(current);
                }

                return;
            }

            open_pq.pop();
            open_set[current.first + current.second * map_width] = false;

            auto [x, y] = current;

            neighbours[0] = { x - 1, y };
            neighbours[1] = { x + 1, y };
            neighbours[2] = { x, y - 1 };
            neighbours[3] = { x, y + 1 };

            for (auto i = 0; i < 4; ++i) {
                auto& neighbour = neighbours[i];
                auto& [x_n, y_n] = neighbour;
                if (!map.at(x_n + y_n * map_width)) {
                    continue;
                }

                auto tentative_g_score = g_score.at(x + y * map_width) + map.at(x + y * map_width);
                if (tentative_g_score >= g_score.at(x_n + y_n * map_width)) {
                    continue;
                }
                g_score[x_n + y_n * map_width] = tentative_g_score;

                came_from[x_n + y_n * map_width] = current;
                if (!open_set[x_n + y_n * map_width]) {
                    open_pq.push({ tentative_g_score + heuristic(neighbour, end), neighbour });
                    open_set[x_n + y_n * map_width] = true;
                }
            }
        }
    }
}

namespace {
    inline int heuristic(const vec2& a, const vec2& b) {
        return std::abs(a.first - b.first) + std::abs(a.second - b.second);
    }
}