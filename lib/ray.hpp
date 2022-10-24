#ifndef RAY_H
#define RAY_H

#include "base.hpp"

namespace rtc
{

class Ray
{
  public:
    Ray();
    Ray(const point &origin, const vec3 &dir);
    ~Ray();

    point At(double t) const;

  public:
    point origin_;
    vec3 dir_;
};

}; // namespace rtc

#endif