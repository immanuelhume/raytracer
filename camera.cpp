#include "camera.hpp"
#include "base.hpp"

using namespace rtc;

Camera::Camera(const double t_open, const double t_close)
    : look_from_(point(0, 0, 0)), look_at_(point(0, 0, -1)), up_(vec3(0, 1, 0)), vfov_(glm::radians(90.0)),
      aspect_ratio_(16.0 / 9.0), focus_dist_((look_at_ - look_from_).length()), t_open_(t_open), t_close_(t_close)
{
    RefreshAll();
}

Ray Camera::GetRay(const double u, const double v) const
{
    // To simulate depth of field, we will offset the ray's origin by a little bit. This causes intersections outside
    // the focus plane to be slightly off and thus blurred. Intersections on the focus plane are not affected.
    // NOTE: the diskRand method is quite slow
    glm::dvec2 rd = lens_radius_ * glm::diskRand(1.0);
    vec3 offset = rd[0] * u_norm_ + rd[1] * v_norm_;

    point p = viewport_center_ + u * u_ + v * v_; // world coordinate of the uv coord given

    // let the ray returned exist at a random time while this camera's shutter is open
    // TODO don't call rand_double if t_open_ same as t_close_
    double t = rand_double(t_open_, t_close_);
    return Ray(look_from_ + offset, p - look_from_ - offset, t);
}

void Camera::RefreshAll()
{
    w_norm_ = glm::normalize(look_at_ - look_from_);
    lens_radius_ = aperture_ / 2.0;

    u_norm_ = glm::normalize(glm::cross(w_norm_, up_));
    v_norm_ = glm::normalize(glm::cross(w_norm_, u_norm_));

    RefreshViewport();
}

void Camera::RefreshViewport()
{
    viewport_height_ = 2.0 * focus_dist_ * glm::tan(vfov_ / 2);
    viewport_width_ = aspect_ratio_ * viewport_height_;
    viewport_center_ = look_from_ + focus_dist_ * w_norm_;

    u_ = (viewport_width_ / 2.0) * u_norm_;
    v_ = (viewport_height_ / 2.0) * v_norm_;
}
