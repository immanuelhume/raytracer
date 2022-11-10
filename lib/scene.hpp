#ifndef SCENE_H
#define SCENE_H

#include "base.hpp"
#include "camera.hpp"
#include "hittable_list.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "thread_pool.hpp"

namespace rtc
{

using RayColor = std::function<color(const Ray &, const HittableList &, int depth)>;

class Scene
{
  public:
    // Creates an empty scene with camera at origin.
    Scene();
    ~Scene();
    void Render(Image &image);

    // Performs arbitrary changes to the scene's camera, and refreshes its geometry.
    void UpdateCamera(std::function<void(Camera &)> f);

  public:
    int samples_per_pixel_;
    int max_depth_;

    HittableList world_;

    RayColor ray_color_;

  private:
    // These are determined by the dimensions of target image. But we need to for the camera as well.
    int w_, h_;
    Camera camera_;
    ThreadPool thread_pool_;
};

// Literally does nothing.
void SetUpBlankScene(Scene &);

/*****************************
 * Final scene from book one *
 *****************************/

// The default algorithm to compute a ray color, used in book one. A ray which does not hit any object
// is rendered as part of a light blue sky.
color RayColor_1(const Ray &ray, const HittableList &world, int depth);
void AddRandomObjects_1(HittableList &list);
void SetUpCamera_1(Camera &);
// A convenience function which recreates the scene from the end of Shirley's first book.
void SetUpScene_1(Scene &);

} // namespace rtc

#endif