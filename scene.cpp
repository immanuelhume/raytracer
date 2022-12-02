#include "scene.hpp"
#include "aarect.hpp"
#include "base.hpp"
#include "box.hpp"
#include "bvh.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "perf_timer.hpp"
#include "sphere.hpp"
#include "static/earthmap.hpp"
#include "transform.hpp"
#include <glm/gtc/random.hpp>

using namespace rtc;

// shades a sky-like background, also acts as a light source!
static rgb SkyBG(const Ray &ray);
// returns solid black background
static rgb BlackBG(const Ray &ray);

const char *rtc::sceneDesc[] = {"Randomly scattered balls", "Randomly scattered balls but some bounce",
                                "Checkered texture demo",   "Perlin noise demo",
                                "Earth image map",          "Cornell box"};
const std::function<void(Scene &s)> rtc::scenes[] = {
    RandomBalls, RandomBouncingBalls, CheckeredDemo, NoiseDemo, EarthDemo, CornellBox,
};

// TODO: the multi-threading scenario is not always faster, figure out why
Scene::Scene() : bg_(SkyBG) {}
Scene::Scene(int num_threads) : bg_(SkyBG), thread_pool_(num_threads) {}

void Scene::Render(Image &image)
{
    PerfTimer _("Render");

    int h = image.h(), w = image.w();
    if (h != h_ || w != w_)
    {
        // if the image target does not match our cached dimensions, we'll need to update camera's aspect ratio
        h_ = h;
        w_ = w;
        camera_.aspect_ratio_ = static_cast<double>(w_) / static_cast<double>(h_);
        camera_.RefreshViewport();
    }
    // NOTE: I thought would make the code run faster by minimizing the number of divides in the actual ray tracing
    // loop, but it seems like it makes no difference after all
    double u_factor = 2.0 / (w_ - 1); // multiplication factor to transform i into u (still need to subtract one)
    double v_factor = 2.0 / (h_ - 1); // ditto
    double per_sample = 1.0 / samples_per_pixel_; // weight per sample
#if 1
    // std::vector<std::thread> threads;
    // for (int n = 0; n < num_threads_; n++)
    // {
    //     threads.emplace_back([&image, this, n, u_factor, v_factor, per_sample]() {
    //         for (int i = n; i < h_; i += this->num_threads_)
    //         {
    //             for (int j = 0; j < w_; j++)
    //             {
    //                 rgb p_color(0);
    //                 for (int s = 0; s < samples_per_pixel_; s++)
    //                 {
    //                     // get our uv coordinates into [-1, 1] range
    //                     double u = (j + rand_double()) * u_factor - 1;
    //                     double v = (i + rand_double()) * v_factor - 1;
    //                     p_color += RayColor(camera_.GetRay(u, v), max_depth_);
    //                 }
    //                 p_color *= per_sample;
    //                 // what is this? gamma correction?
    //                 image.SetPixel(i, j, glm::sqrt(p_color[0]), glm::sqrt(p_color[1]), glm::sqrt(p_color[2]), 1);
    //             }
    //         }
    //     });
    // }
    // for (auto &thread : threads)
    //     if (thread.joinable()) thread.join();

    // thread k will render every kth row
    int nt = thread_pool_.NumThreads();
    for (int n = 0; n < nt; n++)
    {
        thread_pool_.Add([&image, this, n, nt, u_factor, v_factor, per_sample]() {
            for (int i = n; i < h_; i += nt)
            {
                for (int j = 0; j < w_; j++)
                {
                    rgb p_color(0);
                    for (int s = 0; s < samples_per_pixel_; s++)
                    {
                        // get our uv coordinates into [-1, 1] range
                        double u = (j + rand_double()) * u_factor - 1;
                        double v = (i + rand_double()) * v_factor - 1;
                        p_color += RayColor(camera_.GetRay(u, v), max_depth_);
                    }
                    p_color *= per_sample;
                    // what is this? gamma correction?
                    image.SetPixel(i, j, glm::sqrt(p_color[0]), glm::sqrt(p_color[1]), glm::sqrt(p_color[2]), 1);
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
            rgb p_color(0);
            for (int s = 0; s < samples_per_pixel_; s++)
            {
                // get our uv coordinates into [-1, 1] range
                double u = (j + rand_double()) * u_factor - 1;
                double v = (i + rand_double()) * v_factor - 1;
                p_color += RayColor(camera_.GetRay(u, v), max_depth_);
            }
            p_color *= per_sample;
            // what is this? gamma correction?
            image.SetPixel(i, j, glm::sqrt(p_color[0]), glm::sqrt(p_color[1]), glm::sqrt(p_color[2]), 1);
        }
    }
#endif
}

void Scene::UpdateCamera(std::function<void(Camera &)> f)
{
    f(camera_);
    camera_.RefreshAll();
}

rgb Scene::RayColor(const Ray &ray, int depth)
{
    if (depth <= 0) return rgb(0, 0, 0);

    HitRecord rec;

    if (world_.Hit(ray, 0.001, infinity, rec))
    {
        Ray scattered;
        rgb attenuation;
        rgb emitted = rec.mat_->Emit(rec.u_, rec.v_, rec.point_);
        if (rec.mat_->Scatter(ray, rec, attenuation, scattered))
            return emitted + attenuation * RayColor(scattered, depth - 1);

        // The ray was completely absorbed. Return just the emitted light.
        return emitted;
    }

    return bg_(ray);
}

void rtc::BlankScene(Scene &) {}

// from the first book
void AddRandomObjectsImpl(HittableList &list, bool bounce)
{
    // make a big ground
    // color ground_color = color(0.5, 0.5, 0.5, 1);
    auto ground_color = std::make_shared<Checkers>(rgb(0.2, 0.3, 0.1), rgb(0.9, 0.9, 0.9));
    auto ground_material = std::make_shared<Lambertian>(ground_color);
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

    auto material2 = std::make_shared<Lambertian>(rgb(0.4, 0.2, 0.1));
    auto center2 = std::make_shared<Stationary>(point(-4, 1, 0));
    list.Add(std::make_shared<Sphere>(1.0, center2, material2));

    auto material3 = std::make_shared<Metal>(rgb(0.7, 0.6, 0.5), 0.0);
    auto center3 = std::make_shared<Stationary>(point(4, 1, 0));
    list.Add(std::make_shared<Sphere>(1.0, center3, material3));
}

void rtc::RandomBalls(Scene &scene)
{
    HittableList world;
    AddRandomObjectsImpl(world, false);
    scene.world_.Add(std::make_shared<BVHNode>(world, 0, 1.0));

    scene.UpdateCamera([](Camera &c) {
        c.look_from_ = point(13, 2, 3);
        c.look_at_ = point(0, 0, 0);
        c.vfov_ = glm::radians(20.0);
        c.focus_dist_ = 10.0;
        c.t_open_ = 0;
        c.t_close_ = 1;
    });
}

void rtc::RandomBouncingBalls(Scene &scene)
{
    HittableList world;
    AddRandomObjectsImpl(world, true);
    scene.world_.Add(std::make_shared<BVHNode>(world, 0, 1.0));

    scene.UpdateCamera([](Camera &c) {
        c.look_from_ = point(13, 2, 3);
        c.look_at_ = point(0, 0, 0);
        c.vfov_ = glm::radians(20.0);
        c.focus_dist_ = 10.0;
        c.t_open_ = 0;
        c.t_close_ = 1;
    });
}

void rtc::CheckeredDemo(Scene &scene)
{
    // adds two large checkered spheres
    auto checkers = std::make_shared<Checkers>(rgb(0.2, 0.3, 0.1), rgb(0.9, 0.9, 0.9));
    auto mat = std::make_shared<Lambertian>(checkers);
    scene.world_.Add(std::make_shared<Sphere>(10, point(0, -10, 0), std::make_shared<Lambertian>(checkers)));
    scene.world_.Add(std::make_shared<Sphere>(10, point(0, 10, 0), std::make_shared<Lambertian>(checkers)));

    scene.UpdateCamera([](Camera &c) {
        c.look_from_ = point(13, 2, 3);
        c.look_at_ = point(0, 0.0, 0);
        c.vfov_ = glm::radians(20.0);
        c.aperture_ = 0.0;
        c.focus_dist_ = 10.0;
    });
}

void rtc::NoiseDemo(Scene &scene)
{
    auto turb = std::make_shared<Lambertian>(std::make_shared<Turbulence>(2.5));
    auto marb = std::make_shared<Lambertian>(std::make_shared<Marble>(Axis::z, 8));
    rgb nutmeg = rgb(0.398, 0.199, 0.0);
    auto wood = std::make_shared<Lambertian>(std::make_shared<Wood>(nutmeg, 2, 18));

    scene.world_.Add(std::make_shared<Sphere>(1000, point(0, -1000, 0), turb));

    scene.world_.Add(std::make_shared<Sphere>(1.5, point(1, 1.5, 0), turb));

    scene.world_.Add(std::make_shared<Sphere>(0.5, point(7, 0.5, 4.2), marb));
    scene.world_.Add(std::make_shared<Sphere>(0.5, point(6, 0.5, 3), marb));
    scene.world_.Add(std::make_shared<Sphere>(0.5, point(4, 0.5, 4), marb));
    scene.world_.Add(std::make_shared<Sphere>(0.5, point(3, 0.5, 2), marb));
    scene.world_.Add(std::make_shared<Sphere>(0.5, point(1.5, 0.5, 3), marb));
    scene.world_.Add(std::make_shared<Sphere>(0.5, point(0, 0.5, 2), marb));

    scene.UpdateCamera([](Camera &c) {
        c.look_from_ = point(13, 2, 3);
        c.look_at_ = point(0, 0.5, 1.7);
        c.vfov_ = glm::radians(20.0);
        c.focus_dist_ = 10.0;
    });
}

void rtc::EarthDemo(Scene &s)
{
    auto earth_texture = std::make_shared<ImageTexture>(kEarthMap, sizeof kEarthMap);
    auto mat = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(2.0, point(0, 0, 0), mat);

    s.world_.Add(globe);

    s.UpdateCamera([](Camera &c) {
        c.look_from_ = point(13, 2, 3);
        c.look_at_ = point(0, 0, 0);
        c.vfov_ = glm::radians(20.0);
        c.focus_dist_ = 10.0;
    });
}

void rtc::CornellBox(Scene &s)
{
    auto red = std::make_shared<Lambertian>(rgb(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(rgb(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(rgb(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(rgb(15, 15, 15));

    auto left_wall = std::make_shared<YZRect>(0, 555, 0, 555, 555, green);
    auto right_wall = std::make_shared<YZRect>(0, 555, 0, 555, 0, red);
    auto back_wall = std::make_shared<XYRect>(0, 555, 0, 555, 555, white);
    auto floor = std::make_shared<XZRect>(0, 555, 0, 555, 0, white);
    auto ceil = std::make_shared<XZRect>(0, 555, 0, 555, 555, white);
    auto lamp = std::make_shared<XZRect>(213, 343, 227, 332, 554, light);

    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(point(0, 0, 0), point(165, 330, 165), white);
    box1 = std::make_shared<Rotate>(box1, Axis::y, glm::radians(15.0));
    box1 = std::make_shared<Translate>(box1, vec3(265, 0, 295));

    std::shared_ptr<Hittable> box2 = std::make_shared<Box>(point(0, 0, 0), point(165, 165, 165), white);
    box2 = std::make_shared<Rotate>(box2, Axis::y, glm::radians(-18.0));
    box2 = std::make_shared<Translate>(box2, vec3(130, 0, 65));

    s.world_.Add(left_wall);
    s.world_.Add(right_wall);
    s.world_.Add(back_wall);
    s.world_.Add(floor);
    s.world_.Add(ceil);
    s.world_.Add(lamp);
    s.world_.Add(box1);
    s.world_.Add(box2);

    s.bg_ = BlackBG;

    s.UpdateCamera([](Camera &c) {
        c.look_from_ = point(278, 278, -800);
        c.look_at_ = point(278, 278, 0);
        c.vfov_ = glm::radians(40.0);
        c.focus_dist_ = 10.0;
    });
}

static rgb SkyBG(const Ray &ray)
{
    vec3 dir = glm::normalize(ray.dir_);
    double t = 0.5 * (dir[1] + 1.0);
    return (1 - t) * rgb(1.0, 1.0, 1.0) + t * rgb(0.5, 0.7, 1.0);
}

static rgb BlackBG(const Ray &ray)
{
    static rgb black = rgb(0, 0, 0);
    return black;
}