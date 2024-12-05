#include "Sphere.h"

std::optional<float> Sphere::intersect(const Ray& ray) const
{
	glm::vec3 oc = ray.m_origin - m_center;
	float a = glm::dot(ray.m_dir, ray.m_dir); //一般为1
	float b = 2 * glm::dot(oc, ray.m_dir);
	float c = glm::dot(oc, oc) - m_radius * m_radius;
	float d = b * b - 4 * a * c;
	if (d < 0) { return {}; }
	// 计算两个解
	float sqrtD = glm::sqrt(d);
	float t1 = (-b - sqrtD) / (2.0f * a);
	float t2 = (-b + sqrtD) / (2.0f * a);

	// 返回最近的正解
	if (t1 > 0) {
		return t1;
	}
	if (t2 > 0) {
		return t2;
	}
	return {};
}

bool Sphere::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const
{
	glm::vec3 oc = r.m_origin - m_center;
	float a = glm::dot(r.m_dir, r.m_dir);
	float half_b = glm::dot(oc, r.m_dir);
	float c = glm::dot(oc, oc) - m_radius * m_radius;
	float discriminant = half_b * half_b - a * c;

	if (discriminant > 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			glm::vec3 outward_normal = (rec.p - m_center) / m_radius;
			rec.set_face_normal(r, outward_normal);
			rec.matPtr = m_matPtr;
			return true;
		}
		temp = (-half_b + root) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			glm::vec3 outward_normal = (rec.p - m_center) / m_radius;
			rec.set_face_normal(r, outward_normal);
			rec.matPtr = m_matPtr;
			return true;
		}
	}
	return false;
}

bool Plane::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const
{
	float t = glm::dot(m_point - r.m_origin, m_normal) / glm::dot(r.m_dir, m_normal);
	if (t > tmin && t < tmax) {
		rec.t = t;
		rec.p = r.at(rec.t);
		rec.set_face_normal(r, m_normal);
		rec.matPtr = m_matPtr;
		return true;
	}
	return false;
}
