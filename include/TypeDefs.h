#pragma once
#include <Game/ActionType.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>

typedef std::pair<char, char> vec2;

namespace std {
    template<>
    struct hash<vec2> {
        size_t operator()(const vec2& vec) const {
            return vec.first + (vec.second * 128);
        }
    };
}