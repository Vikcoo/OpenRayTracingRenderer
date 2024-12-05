#pragma once

#include <glm/glm.hpp>

class Ray {
public:
    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : m_origin(origin), m_dir(direction) {}
    
    glm::vec3 at(float t) const {
        return m_origin + t * m_dir;
    }
public:
    glm::vec3 m_origin;
    glm::vec3 m_dir;
};