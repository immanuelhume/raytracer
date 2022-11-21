#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "positioner.hpp"

namespace rtc
{

class Sphere : public Hittable
{
    friend class SphereMover;

  public:
    Sphere(double radius, std::shared_ptr<Positioner> position, std::shared_ptr<Material> mat);
    ~Sphere() override;

    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;

  public:
    double radius_;
    std::shared_ptr<Positioner> position_;
    std::shared_ptr<Material> mat_;
};

} // namespace rtc

#endif