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

class Scene
{
  public:
    // Creates an empty scene with camera at origin. The sky will be shaded to light blue.
    Scene();
    Scene(int num_threads);
    void Render(Image &image);

    // Performs arbitrary changes to the scene's camera, and refreshes its geometry. When used to initialize a camera,
    // the most important fields are:
    //
    //   - look_from_
    //   - look_at_
    //   - vfov
    //   - focus_dist_
    //
    // The vertical fov and focus distance determines the camera's viewport height. The aspect ratio is determined only
    // when the scene is rendered to an image (in Scene::Render), and is determined by the image dimensions.
    void UpdateCamera(std::function<void(Camera &)> f);

    // meat of the ray tracer
    rgb RayColor(const Ray &ray, int depth);

  public:
    int max_depth_ = MAX_DEPTH_DEFAULT;
    int samples_per_pixel_ = SAMPLES_PER_PIXEL_DEFAULT;

    HittableList world_;
    std::function<rgb(const Ray &r)> bg_; // function to retrieve background color given some ray from the camera

  private:
    // Determined by the dimensions of target image. But we need to for the camera as well.
    int w_, h_;
    Camera camera_;
    ThreadPool thread_pool_;
};

// Literally does nothing.
void BlankScene(Scene &);

// A convenience function which recreates the scene from the end of Shirley's first book.
void RandomBalls(Scene &);

// Identical to the first scene, but adds some bouncing motion.
void RandomBouncingBalls(Scene &);

// Third scene of the book to test checkered textures.
void CheckeredDemo(Scene &);

// Simple scene demonstrating perlin noise.
void NoiseDemo(Scene &);

// Mapping an earth texture onto a sphere.
void EarthDemo(Scene &);

void CornellBox(Scene &);

// final scene of book 2 (the next week)
void Book2(Scene &);

enum SceneDemo
{
    kRandomBalls = 0,
    kRandomBouncingBalls,
    kCheckered,
    kNoise,
    kEarth,
    kCornellBox,
    kBook2,

    kLast, // helps keep track of how many scenes we have
};

// human friendly descriptions of each scene, indexed by SceneDemo enum
extern const char *sceneDesc[SceneDemo::kLast];

// the actual scene setup functions, indexed by SceneDemo enum
extern const std::function<void(Scene &s)> scenes[SceneDemo::kLast];

} // namespace rtc

#endif