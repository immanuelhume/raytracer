#ifndef MATERIAL_H
#define MATERIAL_H

#include "base.hpp"
#include "hittable.hpp"
#include "ray.hpp"

namespace rtc
{

class Material
{
  public:
    Material() = default;
    virtual ~Material() = default;

    // Tries to compute a scattered ray from from intersection point, and returns true if such a scattered ray was
    // produced.
    virtual bool scatter(const Ray &ray, const HitRecord &rec, color &attenuation, Ray &scattered) const = 0;
};

} // namespace rtc

#endif