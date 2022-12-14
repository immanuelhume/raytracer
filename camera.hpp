#ifndef CAMERA_H
#define CAMERA_H

#include "base.hpp"
#include "ray.hpp"

namespace rtc
{

class Camera
{
  public:
    // creates a camera at the origin, looking at negative z
    Camera(double t_open = 0, double t_close = 0);

    // generates a ray from the camera's origin through some (u, v) on its viewport, where (0, 0) is the viewport's
    // center
    Ray GetRay(double u, double v) const;

    // any changes to camera positions do not take effect until RefreshAll is called
    void RefreshAll();

    // account for changes to viewport dimension, i.e. field of view or aspect ratio
    void RefreshViewport();

  public:
    point look_from_;
    point look_at_;
    point up_;

    // verticle field of view, in radians
    double vfov_;
    double aspect_ratio_;
    double aperture_ = 0.1;
    double focus_dist_;
    // point in time where the shutter opens
    double t_open_;
    // point in time where the shutter closes
    double t_close_;

  private:
    double viewport_height_;
    double viewport_width_;
    point viewport_center_;

    // pointing towards what we are looking at
    vec3 w_norm_;

    // u vector, half of viewport width (viewport →)
    vec3 u_;
    vec3 u_norm_;

    // v vector, half of viewport height (viewport ↓)
    vec3 v_;
    vec3 v_norm_;

    double lens_radius_;
};

} // namespace rtc

#endif