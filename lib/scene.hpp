#ifndef SCENE_H
#define SCENE_H

#include "base.hpp"
#include "camera.hpp"
#include "hittable_list.hpp"
#include "image.hpp"
#include "ray.hpp"

namespace rtc
{

class Scene
{
  public:
    Scene();
    ~Scene();
    void Render(Image &image);

  private:
    color RayColor(const Ray &ray, int depth) const; // determines the color of this ray

  public:
    int samples_per_pixel_;
    int max_depth_;
    HittableList world_;

  private:
    int w_, h_;
    Camera camera_;
};

} // namespace rtc

#endif