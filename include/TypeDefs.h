#pragma once
#include <utility>

typedef std::pair<int, int> vec2;

constexpr vec2 vec2_null = vec2(-1, -1);

namespace {
    int vec_to_index(const vec2& vec, int width) {
        return vec.first + vec.second * width;
    }
}