#ifndef CAMERA_H
#define CAMERA_H

#include "base.hpp"
#include "ray.hpp"

namespace rtc
{

class Camera
{
  public:
    Camera(); // creates a camera at the origin, looking at negative z

    Ray GetRay(double u, double v) const;
    void Refresh(); // any changes to camera properties do not take effect until Refresh is called

  public:
    point look_from_;
    point look_at_;
    point up_;

    double vfov_; // verticle field of view, in radians
    double aperture_;
    double aspect_ratio_;
    double focus_dist_;

  private:
    double viewport_height_;
    double viewport_width_;
    point viewport_center_;
    vec3 w_norm_; // pointing towards what we are looking at
    vec3 u_;      // u vector (canvas →)
    vec3 u_norm_;
    vec3 v_; // v vector (canvas ↓)
    vec3 v_norm_;
    double lens_radius_;
};

} // namespace rtc

#endif