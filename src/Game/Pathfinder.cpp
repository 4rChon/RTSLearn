#include <Game/Pathfinder.h>
#include <TypeDefs.h>
#include <unordered_map>
#include <unordered_set>
#include <queue>

Vec2i Pathfinder::neighbours[4];

std::vector<Vec2i> Pathfinder::get_path(const Vec2i& start, const Vec2i& end, std::vector<std::vector<int>> map) {
    const int map_width = map.size();
    const int map_height = map[0].size();
    std::priority_queue<std::pair<int, Vec2i>, std::vector<std::pair<int, Vec2i>>, std::greater<std::pair<int, Vec2i>>> open_pq;
    open_pq.push({ 0, start });
    
    std::unordered_map<Vec2i, bool> open_set{ {start, true } };

    std::unordered_map<Vec2i, Vec2i> came_from;
    std::unordered_map<Vec2i, int> g_score{ { start, 0 } };
    std::unordered_map<Vec2i, int> f_score{ { start, heuristic(start, end) } };

    while (!open_pq.empty()) {
        auto current = open_pq.top().second;
        if (current == end) {
            return reconstruct_path(came_from, current);
        }
        open_pq.pop();
        open_set[current] = false;

        auto x = current.first;
        auto y = current.second;

        neighbours[0] = { x - 1, y };
        neighbours[1] = { x + 1, y };
        neighbours[2] = { x, y - 1 };
        neighbours[3] = { x, y + 1 };
        
        for (auto i = 0; i < 4; i++) {
            auto neighbour = neighbours[i];
            // check if neighbour is within map bounds
            if (neighbour.first < 0 || neighbour.first >= map_width || neighbour.second < 0 || neighbour.second >= map_height || map[neighbour.first][neighbour.second] == 0) {
                continue;
            }

            auto tentative_g_score = g_score[current] + map[current.first][current.second];
            if (g_score.contains(neighbour) && tentative_g_score >= g_score[neighbour]) {
                continue;
            }
            came_from[neighbour] = current;
            g_score[neighbour] = tentative_g_score;
            f_score[neighbour] = tentative_g_score + heuristic(neighbour, end);
            if (open_set.contains(neighbour) || open_set[neighbour]) {
                continue;
            }
            open_pq.push({ f_score[neighbour], neighbour });
            open_set[neighbour] = true;
        }
    }

    return std::vector<Vec2i>();
}

int Pathfinder::heuristic(const Vec2i& a, const Vec2i& b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

std::vector<Vec2i> Pathfinder::reconstruct_path(std::unordered_map<Vec2i, Vec2i> came_from, Vec2i current)
{
    std::vector<Vec2i> total_path { current };

    while (came_from.find(current) != came_from.end()) {
        current = came_from[current];
        total_path.push_back(current);
    }

    return total_path;
}