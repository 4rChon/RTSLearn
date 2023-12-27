#include <Game/Renderer/RenderBuffer.h>
#include <sstream>

namespace Renderer {
    RenderBuffer::RenderBuffer()
    : pointer_index(0) {
        buffer = std::vector<std::string>({ "" });
    }

    void RenderBuffer::append_line(const std::string& line) {
        buffer[pointer_index] += line;
    }

    void RenderBuffer::new_line() {
        append_line("\x1b[K\n");
        buffer.push_back("");
        pointer_index++;
    }

    bool RenderBuffer::empty() const {
        return buffer.empty() || buffer[0].empty();
    }
    
    void RenderBuffer::clear() {
        buffer.clear();
        buffer.push_back("");
        reset_pointer();
    }

    void RenderBuffer::reset_pointer() {
        pointer_index = 0;
    }

    std::stringstream RenderBuffer::build() const {
        std::stringstream ss;
        for (const auto& line : buffer) {
            ss << line;
        }
        return ss;
    }

    std::string RenderBuffer::read() const {
        return buffer[pointer_index];
    }

    bool RenderBuffer::increment_pointer() {
        if (pointer_index >= buffer.size()) {
            pointer_index = 0;

            return false;
        }

        return ++pointer_index < buffer.size();
    }

    std::string RenderBuffer::get_cursor_position() const {
        std::stringstream ss;
        ss << "\x1b[" << pointer_index + 1 << ";" << 0 << "H";
        return ss.str();
    }
}