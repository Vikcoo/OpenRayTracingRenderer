#pragma once

#include <vector>
#include <filesystem>
#include <glm/glm.hpp>

class FrameBuffer
{
public:
    FrameBuffer(size_t width, size_t height);

    void save(const std::filesystem::path& filename);

    size_t getWidth() const { return m_width; }
    size_t getHeight() const { return m_height; }
    glm::vec3 getPixel(size_t x, size_t y) const { return m_pixels[y * m_width + x]; }
    void setPixel(size_t x, size_t y, const glm::vec3& color) { m_pixels[y * m_width + x] = color; }
private:
    size_t m_width;
    size_t m_height;
    std::vector<glm::vec3> m_pixels;
};
