#include "scene.hpp"
#include "base.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "perf_timer.hpp"
#include "sphere.hpp"
#include <glm/gtc/random.hpp>

using namespace rtc;

// TODO: the multi-threading scenario is not always faster, figure out why
Scene::Scene() : ray_color_(RayColor_1), thread_pool_(std::max((u_int)1, std::thread::hardware_concurrency() / 4))
{
}

Scene::~Scene()
{
}

void Scene::Render(Image &image)
{
    PerfTimer t("Render");

    int h = image.h(), w = image.w();
    if (h != h_ || w != w_)
    {
        // if the image target does not match our cached dimensions, we'll need to update camera's aspect ratio
        h_ = h;
        w_ = w;
        camera_.aspect_ratio_ = static_cast<double>(w_) / static_cast<double>(h_);
        camera_.RefreshViewport();
    }

#define PAR_RENDER
#ifdef PAR_RENDER

    // give each thread gets a couple of contiguous rows to render
    int nt = thread_pool_.NumThreads();
    int dy = h_ / nt;
    for (int n = 0; n < nt; n++)
    {
        int s = n * dy;                    // starting row
        int e = n == nt - 1 ? h_ : s + dy; // ending row (non inclusive)
        thread_pool_.Add([&image, this, s, e]() {
            for (int i = s; i < e; i++)
            {
                for (int j = 0; j < w_; j++)
                {
                    color p_color(0);
                    for (int s = 0; s < samples_per_pixel_; s++)
                    {
                        // get our uv coordinates into [-1, 1] range
                        double u = (j + rand_double()) * 2.0 / static_cast<float>(w_ - 1) - 1;
                        double v = (i + rand_double()) * 2.0 / static_cast<float>(h_ - 1) - 1;
                        p_color += ray_color_(camera_.GetRay(u, v), world_, max_depth_);
                    }
                    p_color /= static_cast<double>(samples_per_pixel_);
                    // what is this? gamma correction?
                    image.SetPixel(i, j, sqrt(p_color[0]), sqrt(p_color[1]), sqrt(p_color[2]), sqrt(p_color[3]));
                }
            }
        });
    }

    thread_pool_.Wait();

#else

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            color p_color(0);
            for (int s = 0; s < samples_per_pixel_; s++)
            {
                // get our uv coordinates into [-1, 1] range
                double u = (j + rand_double()) * 2.0 / static_cast<float>(w_ - 1) - 1;
                double v = (i + rand_double()) * 2.0 / static_cast<float>(h_ - 1) - 1;
                p_color += ray_color_(camera_.GetRay(u, v), world_, max_depth_);
            }
            p_color /= static_cast<double>(samples_per_pixel_); // gamma correction
            image.SetPixel(i, j, sqrt(p_color[0]), sqrt(p_color[1]), sqrt(p_color[2]), sqrt(p_color[3]));
        }
    }

#endif
}

void Scene::UpdateCamera(std::function<void(Camera &)> f)
{
    f(camera_);
    camera_.RefreshAll();
}

void rtc::SetUpBlankScene(Scene &)
{
}

/*****************************
 * Final scene from book one *
 *****************************/

// recursively determines the color of a ray by scattering a ray
color rtc::RayColor_1(const Ray &ray, const HittableList &world, int depth)
{
    if (depth <= 0)
        return color(0, 0, 0, 1);

    HitRecord rec;

    if (world.Hit(ray, 0.001, infinity, rec))
    {
        Ray scattered;
        color attenuation;
        if (rec.mat_->scatter(ray, rec, attenuation, scattered))
            return attenuation * rtc::RayColor_1(scattered, world, depth - 1);

        // The ray was completely absorbed. Return black.
        return color(0, 0, 0, 1);
    }

    // shade a "sky-like" background
    vec3 dir = glm::normalize(ray.dir_);
    double t = 0.5 * (dir[1] + 1.0);
    return (1 - t) * color(1.0, 1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0, 1.0);
}

void AddRandomObjectsImpl(HittableList &list, bool bounce)
{
    // make a big ground
    auto ground_material = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5, 1));
    list.Add(std::make_shared<Sphere>(1000, std::make_shared<Stationary>(point(0, -1000, 0)), ground_material));

    // make many small spheres of radius 0.2
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = rand_double();
            point center(a + 0.9 * rand_double(), 0.2, b + 0.9 * rand_double());

            if ((center - point(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;
                std::shared_ptr<Positioner> position = std::make_shared<Stationary>(center);

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = rand_color() * rand_color();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    if (bounce)
                    {
                        // Creates a bounce with a period of 8 and amplitude 0.5. Randomly choose a time within [0, 8]
                        // to "drop the ball".
                        position = std::make_shared<Parabolic>(8, rand_double(0, 8), center, center + vec3(0, 0.5, 0));
                        // TODO: no need to use rand_double for the start time here, just use a random bag shuffle thing
                    }
                    list.Add(std::make_shared<Sphere>(0.2, position, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = rand_color(0.5, 1);
                    auto fuzz = rand_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    list.Add(std::make_shared<Sphere>(0.2, position, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    list.Add(std::make_shared<Sphere>(0.2, position, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    auto center1 = std::make_shared<Stationary>(point(0, 1, 0));
    list.Add(std::make_shared<Sphere>(1.0, center1, material1));

    auto material2 = std::make_shared<Lambertian>(color(0.4, 0.2, 0.1, 1));
    auto center2 = std::make_shared<Stationary>(point(-4, 1, 0));
    list.Add(std::make_shared<Sphere>(1.0, center2, material2));

    auto material3 = std::make_shared<Metal>(color(0.7, 0.6, 0.5, 1), 0.0);
    auto center3 = std::make_shared<Stationary>(point(4, 1, 0));
    list.Add(std::make_shared<Sphere>(1.0, center3, material3));
}

void AddRandomObjects_1(HittableList &list)
{
    AddRandomObjectsImpl(list, false);
}

void AddRandomObjects_2(HittableList &list)
{
    AddRandomObjectsImpl(list, true);
}

void SetUpCamera_1(Camera &c)
{
    c.look_from_ = point(13, 2, 3);
    c.look_at_ = point(0, 0, 0);
    c.vfov_ = glm::radians(20.0);
    c.focus_dist_ = 10.0;
    c.t_open_ = 0;
    c.t_close_ = 1;
    c.RefreshAll();
}

void rtc::SetUpScene_1(Scene &scene)
{
    scene.ray_color_ = RayColor_1;
    AddRandomObjects_1(scene.world_);
    scene.UpdateCamera(SetUpCamera_1);
}

void rtc::SetUpScene_2(Scene &scene)
{
    scene.ray_color_ = RayColor_1;
    AddRandomObjects_2(scene.world_);
    scene.UpdateCamera(SetUpCamera_1);
}