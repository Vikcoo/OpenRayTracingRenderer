#pragma once

#include "Ray.h"
#include "Hittable.h"
#include <optional>

class Sphere : public Hittable {
public:
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> matPtr) : m_center(center), m_radius(radius), m_matPtr(matPtr) {}

	std::optional<float> intersect(const Ray& ray) const;

	virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const;

public:
	glm::vec3 m_center;
	float m_radius;
	std::shared_ptr<Material> m_matPtr;
};

class Plane : public Hittable {
public:
	Plane(const glm::vec3& point, const glm::vec3& normal, std::shared_ptr<Material> matPtr) : m_point(point), m_normal(normal), m_matPtr(matPtr) {}
	
	virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const;

public:
	glm::vec3 m_point;
	glm::vec3 m_normal;
	std::shared_ptr<Material> m_matPtr;
};