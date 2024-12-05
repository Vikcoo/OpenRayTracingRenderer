#pragma once
#include <cstdlib>
#define PI 3.14159265358979323846

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

inline double random_double() {
    // Returns a random real in [0,1).
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline glm::vec3 random(double min, double max) {
    return glm::vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

glm::vec3 random_in_unit_sphere() {
    while (true) {
        auto p = random(-1, 1);
        if (glm::length(p) >= 1) continue;
        return p;
    }
}
glm::vec3 random_unit_vector() {
    auto a = random_double(0, 2 * PI);
    auto z = random_double(-1, 1);
    auto r = sqrt(1 - z * z);
    return glm::vec3(r * cos(a), r * sin(a), z);
}

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) {
    return v - 2 * glm::dot(v, n) * n;
}

glm::vec3 refract(const glm::vec3& uv, const glm::vec3& n, float etai_over_etat) {
    float cos_theta = dot(-uv, n);
    glm::vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    glm::vec3 r_out_perp = static_cast<float>(-glm::sqrt(1.0 - glm::length(r_out_parallel))) * n;
    return r_out_parallel + r_out_perp;
}

double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}