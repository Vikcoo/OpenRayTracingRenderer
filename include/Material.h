#pragma once
#include "Tool.h"
#include "Ray.h"
#include "Hittable.h"
#include "glm/glm.hpp"
class Material {
public:
    virtual bool scatter(const Ray& r, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const glm::vec3& a) : m_albedo(a) {}
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const {
        glm::vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = Ray(rec.p, scatter_direction);
        attenuation = m_albedo;
        return true;
    }
public:
	glm::vec3 m_albedo;
};


class Metal : public Material {
public:
    Metal(const glm::vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
    ) const {
        glm::vec3 reflected = reflect(glm::normalize(r_in.m_dir), rec.normal);
        scattered = Ray(rec.p, reflected + (float)fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (glm::dot(scattered.m_dir, rec.normal) > 0);
    }

public:
    glm::vec3 albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double ri) : ref_idx(ri) {}

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
    ) const {
        attenuation = glm::vec3(1.0, 1.0, 1.0);
        double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);

        glm::vec3 unit_direction = glm::normalize(r_in.m_dir);
        double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        if (etai_over_etat * sin_theta > 1.0) {
            glm::vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = Ray(rec.p, reflected);
            return true;
        }

        glm::vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = Ray(rec.p, refracted);
        return true;
    }

public:
    double ref_idx;
};