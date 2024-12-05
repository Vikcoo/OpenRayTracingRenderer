#include <iostream>
#include <Chrono>
#include <glm/glm.hpp>
#include <ThreadPool.h>
#include <FrameBuffer.h>
#include <memory>
#include "Camera.h"
#include "Sphere.h"
#include "Hittable.h"
#include "Tool.h"
#include "Material.h"

class SimpleTask : public Task
{
public:
   void run() override
   {
      std::cout << "SimpleTask::run()\n"  << std::endl;
   }
};


glm::vec3 rayColor(const Ray& r, const HittableList& world, int depth) {
    HitRecord rec;
    if (depth <= 0) { return glm::vec3(0.0, 0.0, 0.0); }

    if (world.hit(r, 0.001, FLT_MAX, rec)) {
        Ray scattered(glm::vec3(0.0), glm::vec3(0.0));
        glm::vec3 attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * rayColor(scattered, world, depth - 1);
        }
        return glm::vec3(0.0, 0.0, 0.0);
    }

    glm::vec3 unit_direction = glm::normalize(r.m_dir);

    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
    FrameBuffer fb(400, 300);
    ThreadPool tp(0);
    Camera camera(fb, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 90);
    HittableList hittableList;

    hittableList.add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Lambertian>(glm::vec3(0.7f, 0.3f, 0.3f))));
    //hittableList.add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f))));

    hittableList.add(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.0f)));
    //hittableList.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.5f)));
    hittableList.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Dielectric>(1.5f)));
    hittableList.add(std::make_shared<Plane>(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.0f)));


    int depth = 50;

    tp.parallelFor(fb.getWidth(), fb.getHeight(), [&](size_t x, size_t y) {
        
        glm::vec3 color = glm::vec3(0.0f);
        int samples_per_pixel = 30;
        for (int s = 0; s < samples_per_pixel; ++s) {
            glm::vec2 offset = glm::vec2(random_double(0.0f, 1.0f), random_double(0.0f, 1.0f));
            Ray ray = camera.generateRay(glm::ivec2(x, y), offset);
            color += rayColor(ray, hittableList, depth);
        }
        color /= samples_per_pixel;
        color.r = glm::sqrt(color.r);
        color.g = glm::sqrt(color.g);
        color.b = glm::sqrt(color.b);
        fb.setPixel(x, y, color);
    });
    tp.wait();

    auto now = std::chrono::high_resolution_clock::now();
    fb.save("test.ppm");
    auto time = std::chrono::high_resolution_clock::now() - now;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
    std::cout << "Time taken: " << ms << "ms" << std::endl;
    return 0;
}


