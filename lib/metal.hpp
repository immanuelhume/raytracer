#ifndef METAL_H
#define METAL_H

#include "material.hpp"

namespace rtc
{

class Metal : public Material
{
  public:
    Metal();
    Metal(const glm::dvec4 &albedo, double fuzz);
    virtual ~Metal() override;

    virtual bool scatter(const Ray &ray, const HitRecord &rec, color &attenuation, Ray &scattered) const override;

  public:
    color albedo_;

    // degree of randomness in direction of scattered ray, 0 for a mirror-like look and 1 for an almost diffuse look
    double fuzz_;
};

} // namespace rtc

#endif