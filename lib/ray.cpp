#include "ray.hpp"

using namespace rtc;

Ray::Ray()
{
}

Ray::Ray(const point &origin, const vec3 &dir) : origin_(origin), dir_(dir)
{
}

Ray::~Ray()
{
}

point Ray::At(const double t) const
{
    return origin_ + t * dir_;
}
