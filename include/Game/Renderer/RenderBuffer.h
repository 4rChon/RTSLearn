#pragma once
#include <vector>
#include <string>
#include <sstream>

namespace Renderer {
    class RenderBuffer {
    public:
        int pointer_index;

        RenderBuffer();
        void append_line(const std::string& line);
        void new_line();
        bool empty() const;
        void clear();
        void reset_pointer();
        std::stringstream build() const;
        std::string read() const;
        bool increment_pointer();
        std::string get_cursor_position() const;
    private:
        std::vector<std::string> buffer;
    };
}
