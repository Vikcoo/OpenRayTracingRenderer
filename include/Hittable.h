#pragma once
#include <vector>
#include <memory>
#include "Ray.h"

class Material;

struct HitRecord {
    glm::vec3 p;
    glm::vec3 normal;
    double t;
    bool front_face;
    std::shared_ptr<Material> matPtr;

    inline void set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
        front_face = glm::dot(r.m_dir, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;

public:

};

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const;

public:
    std::vector<std::shared_ptr<Hittable>> objects;
};