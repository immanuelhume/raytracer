#ifndef AABB_H
#define AABB_H

#include "base.hpp"
#include "ray.hpp"

namespace rtc
{

class AABB
{
  public:
    AABB() = default;
    AABB(point min, point max);

    bool Hit(const Ray &ray, double t_min, double t_max) const;
    static AABB Surround(const AABB &a, const AABB &b);

  public:
    point min_ = point(0, 0, 0);
    point max_ = point(0, 0, 0);
};

} // namespace rtc

#endif