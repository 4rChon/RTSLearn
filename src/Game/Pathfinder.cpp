#include <Game/Pathfinder.h>
#include <TypeDefs.h>
#include <unordered_map>
#include <queue>

namespace Pathfinder {
    void Pathfinder::get_path(const vec2& start, const vec2& end, const std::vector<std::vector<bool>>& map, std::vector<vec2>& out_path) {
        const unsigned char map_width = map.size();
        const unsigned char map_height = map[0].size();
        std::priority_queue<std::pair<unsigned char, vec2>, std::vector<std::pair<unsigned char, vec2>>, std::greater<std::pair<unsigned char, vec2>>> open_pq;
        open_pq.push({ 0, start });

        std::unordered_map<vec2, bool> open_set{ {start, true } };
        std::unordered_map<vec2, vec2> came_from;
        std::vector<std::vector<unsigned short>> g_score = std::vector<std::vector<unsigned short>>(map_width, std::vector<unsigned short>(map_height, std::numeric_limits<unsigned short>::max()));
        std::vector<std::vector<unsigned short>> f_score = std::vector<std::vector<unsigned short>>(map_width, std::vector<unsigned short>(map_height, std::numeric_limits<unsigned short>::max()));

        g_score[start.first][start.second] = 0;
        g_score[start.first][start.second] = heuristic(start, end);
        
        came_from.reserve(map_width * map_height);
        open_set.reserve(map_width * map_height);

        while (!open_pq.empty()) {
            auto current = open_pq.top().second;
            if (current == end) {
                out_path.push_back(current);

                while (came_from.contains(current)) {
                    current = came_from[current];
                    out_path.push_back(current);
                }

                return;
            }

            open_pq.pop();
            open_set[current] = false;

            auto x = current.first;
            auto y = current.second;

            neighbours[0] = { x - 1, y };
            neighbours[1] = { x + 1, y };
            neighbours[2] = { x, y - 1 };
            neighbours[3] = { x, y + 1 };

            for (auto i = 0; i < 4; ++i) {
                auto& neighbour = neighbours[i];
                auto x_n = neighbour.first;
                auto y_n = neighbour.second;
                if (!map[x_n][y_n]) {
                    continue;
                }

                auto tentative_g_score = g_score[x][y] + map[x][y];
                if (tentative_g_score >= g_score[x_n][y_n]) {
                    continue;
                }

                came_from[neighbour] = current;
                g_score[x_n][y_n] = tentative_g_score;
                f_score[x_n][y_n] = tentative_g_score + heuristic(neighbour, end);
                if (open_set[neighbour]) {
                    continue;
                }
                open_pq.push({ f_score[x_n][y_n], neighbour});
                open_set[neighbour] = true;
            }
        }
    }
}

namespace {
    inline int heuristic(const vec2& a, const vec2& b) {
        return std::abs(a.first - b.first) + std::abs(a.second - b.second);
    }
}