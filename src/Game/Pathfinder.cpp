#include <Game/Pathfinder.h>
#include <TypeDefs.h>
#include <unordered_map>
#include <queue>
#include <vector>
#include <array>
#include <Game/Map.h>

namespace Pathfinder {
    void Pathfinder::get_path(const vec2& start, const vec2& end, const Map& map, std::vector<vec2>& out_path) {
        const auto& [map_width, map_height] = map.get_size();

        std::priority_queue<std::pair<int, vec2>, std::vector<std::pair<int, vec2>>, std::greater<std::pair<int, vec2>>> open_pq;
        open_pq.push({ 0, start });

        auto open_set = std::vector<bool>(map_width * map_height);
        auto came_from = std::vector<vec2>(map_width * map_height, vec2_null);
        auto g_score = std::vector<int>(map_width * map_height, std::numeric_limits<int>::max());

        const auto start_index = start.first + start.second * map_width;
        g_score[start_index] = 0;
        open_set[start_index] = true;


        while (!open_pq.empty()) {
            auto current = open_pq.top().second;
            const auto current_index = current.first + current.second * map_width;

            if (current == end) {
                out_path.push_back(current);

                auto next_index = current.first + current.second * map_width;
                while (came_from[next_index].first != -1) {
                    current = came_from[next_index];
                    next_index = current.first + current.second * map_width;
                    out_path.push_back(current);
                }

                return;
            }

            open_pq.pop();
            open_set[current_index] = false;

            const auto [x, y] = current;
            const auto& neighbours = map.get_adjacent_tiles(current);
            for (const auto& neighbour : neighbours) {
                if (neighbour == nullptr || !neighbour->is_pathable()) {
                    continue;
                }

                const auto& neighbour_position = neighbour->get_position();
                const auto nieghbour_index = neighbour_position.first + neighbour_position.second * map_width;

                const auto tentative_g_score = g_score[current_index] + 1;
                if (tentative_g_score >= g_score[nieghbour_index]) {
                    continue;
                }

                g_score[nieghbour_index] = tentative_g_score;

                came_from[nieghbour_index] = current;
                if (!open_set[nieghbour_index]) {
                    open_pq.push({ tentative_g_score + heuristic(neighbour_position, end), neighbour_position });
                    open_set[nieghbour_index] = true;
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