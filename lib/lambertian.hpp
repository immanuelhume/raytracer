#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.hpp"

namespace rtc
{

// a diffuse material
class Lambertian : public Material
{
  public:
    Lambertian();
    Lambertian(const color &albedo);
    virtual ~Lambertian() override;

    virtual bool scatter(const Ray &ray, const HitRecord &rec, color &attenuation, Ray &scattered) const override;

  public:
    color albedo_;
};

} // namespace rtc

#endif