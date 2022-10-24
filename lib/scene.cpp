#include "scene.hpp"
#include "base.hpp"
#include "dielectric.hpp"
#include "hittable.hpp"
#include "lambertian.hpp"
#include "material.hpp"
#include "metal.hpp"
#include "sphere.hpp"
#include <cmath>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <limits>

using namespace rtc;

void AddRandomObjects(HittableList &list)
{
    // make a big ground
    auto ground_material = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5, 1));
    list.Add(std::make_shared<Sphere>(point(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = rand_double();
            point center(a + 0.9 * rand_double(), 0.2, b + 0.9 * rand_double());

            if ((center - point(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = rand_color() * rand_color();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    list.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = rand_color(0.5, 1);
                    auto fuzz = rand_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    list.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    list.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    list.Add(std::make_shared<Sphere>(point(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(color(0.4, 0.2, 0.1, 1));
    list.Add(std::make_shared<Sphere>(point(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(color(0.7, 0.6, 0.5, 1), 0.0);
    list.Add(std::make_shared<Sphere>(point(4, 1, 0), 1.0, material3));
}

Scene::Scene() : samples_per_pixel_(1), max_depth_(50)
{
    camera_.look_from_ = point(13, 2, 3);
    camera_.look_at_ = point(0, 0, 0);
    camera_.vfov_ = glm::radians(20.0);
    camera_.focus_dist_ = 10.0;
    camera_.Refresh();

    AddRandomObjects(world_);
}

Scene::~Scene()
{
}

void Scene::Render(Image &image)
{
    int h = image.h(), w = image.w();
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            color p_color(0);
            for (int s = 0; s < samples_per_pixel_; s++)
            {
                // get our uv coordinates into [-1, 1] range
                double u = (j + rand_double()) * 2.0 / static_cast<float>(w - 1) - 1;
                double v = (i + rand_double()) * 2.0 / static_cast<float>(h - 1) - 1;
                p_color += RayColor(camera_.GetRay(u, v), max_depth_);
            }
            p_color /= static_cast<double>(samples_per_pixel_); // gamma correction
            image.SetPixel(i, j, sqrt(p_color[0]), sqrt(p_color[1]), sqrt(p_color[2]), sqrt(p_color[3]));
        }
    }
}

// recursively determines the color of a ray by scattering a ray
color Scene::RayColor(const Ray &ray, int depth) const
{
    if (depth <= 0)
        return color(0, 0, 0, 1);

    HitRecord rec;

    if (world_.Hit(ray, 0.001, infinity, rec))
    {
        Ray scattered;
        color attenuation;
        if (rec.mat_->scatter(ray, rec, attenuation, scattered))
            return attenuation * RayColor(scattered, depth - 1);

        return color(0, 0, 0, 1);
    }

    // shade a "sky-like" background
    vec3 dir = glm::normalize(ray.dir_);
    double t = 0.5 * (dir[1] + 1.0);
    return (1 - t) * color(1.0, 1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0, 1.0);
}
