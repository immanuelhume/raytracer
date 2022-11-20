#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"

namespace rtc
{

// encapsulates some kind of function defined over real numbers
class Positioner
{
  public:
    virtual point At(double time) = 0;
};

class Stationary : public Positioner
{
  public:
    Stationary(point p);
    virtual point At(double time) override;

  public:
    point p_;
};

class Parabolic : public Positioner
{
  public:
    Parabolic(double period, double start = 0, point low = point(0, 0, 0), point high = point(0, 1, 0));
    virtual point At(double time) override;

  private:
    double a_, b_;
    double start_;
    double period_;
    point low_;
    vec3 dir_;
};

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