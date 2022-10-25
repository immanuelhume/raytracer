#include "camera.hpp"
#include "base.hpp"

using namespace rtc;

Camera::Camera()
    : look_from_(point(0, 0, 0)), look_at_(point(0, 0, -1)), up_(vec3(0, 1, 0)), vfov_(glm::radians(90.0)),
      aspect_ratio_(16.0 / 9.0), aperture_(0.1), focus_dist_((look_at_ - look_from_).length())
{
    RefreshAll();
}

Ray Camera::Camera::GetRay(const double u, const double v) const
{
    glm::dvec2 rd = lens_radius_ * glm::diskRand(1.0);
    vec3 offset = rd[0] * u_norm_ + rd[1] * v_norm_;

    point p = viewport_center_ + u * u_ + v * v_; // world coordinate of the uv coord given
    return Ray(look_from_ + offset, p - look_from_ - offset);
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
