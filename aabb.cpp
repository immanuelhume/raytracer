#include "aabb.hpp"
#include <utility>

using namespace rtc;

AABB::AABB(const point &min, const point &max) : min_(min), max_(max) {}

bool AABB::Hit(const Ray &ray, double t_min, double t_max) const
{
    // Straightforward but tricky at the same time. For a ray with equation P + tv, where P is its origin and v the
    // direction we compute a (t0, t1) where the ray intersects each pair of planes of the box (the box is defined by a
    // min and max point, but this is the same as 6 planes which "bound" the box).
    for (int i = 0; i < 3; i++)
    {
        double inv_d = 1.0 / ray.dir_[i];
        double t0 = (min_[i] - ray.origin_[i]) * inv_d;
        double t1 = (max_[i] - ray.origin_[i]) * inv_d;
        if (inv_d < 0.0) std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min) return false;
    }
    return true;
}

AABB AABB::Surround(const AABB &a, const AABB &b)
{
    double min_x = std::min(a.min_.x, b.min_.x);
    double min_y = std::min(a.min_.y, b.min_.y);
    double min_z = std::min(a.min_.z, b.min_.z);

    double max_x = std::max(a.max_.x, b.max_.x);
    double max_y = std::max(a.max_.y, b.max_.y);
    double max_z = std::max(a.max_.z, b.max_.z);

    return AABB(vec3(min_x, min_y, min_z), vec3(max_x, max_y, max_z));
}