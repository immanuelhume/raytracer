#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.hpp"
namespace rtc
{

class Dielectric : public Material
{
  public:
    Dielectric();
    Dielectric(double refractive_index);
    virtual ~Dielectric() override;
    virtual bool scatter(const Ray &ray, const HitRecord &rec, color &attenuation, Ray &scattered) const override;

  private:
    static double reflectance(double cosine, double ref_idx);

  public:
    double ri_; // the refractive index
};

} // namespace rtc

#endif