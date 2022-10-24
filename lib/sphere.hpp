#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include <memory>

namespace rtc
{

class Sphere : public Hittable
{
  public:
    Sphere();
    Sphere(point center, double radius, std::shared_ptr<Material> mat);
    ~Sphere();

    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;

  public:
    point center_;
    double radius_;
    std::shared_ptr<Material> mat_;
};

} // namespace rtc

#endif