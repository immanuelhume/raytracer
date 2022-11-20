#ifndef RAY_H
#define RAY_H

#include "base.hpp"

namespace rtc
{

class Ray
{
  public:
    Ray(){};
    Ray(const point &origin, const vec3 &dir, const double time = 0) : origin_(origin), dir_(dir), time_(time){};
    ~Ray(){};

    point At(double t) const
    {
        return origin_ + t * dir_;
    }

  public:
    point origin_;
    vec3 dir_;
    // the point in time where this ray exists
    double time_;
};

}; // namespace rtc

#endif