#include "aabb.hpp"
#include <utility>

using namespace rtc;

AABB::AABB(point min, point max) : min_(min), max_(max)
{
}

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
        if (inv_d < 0.0)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
            return false;
    }
    return true;
}

AABB AABB::Surround(const AABB &a, const AABB &b)
{
    return AABB(glm::min(a.min_, b.min_), glm::max(a.max_, b.max_));
}