#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "positioner.hpp"

namespace rtc
{

class Sphere : public Hittable
{
  public:
    Sphere(double radius, const point &center, std::shared_ptr<Material> mat);
    Sphere(double radius, std::shared_ptr<Positioner> position, std::shared_ptr<Material> mat);
    ~Sphere() override = default;

    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

  public:
    double radius_;
    std::shared_ptr<Positioner> position_;
    std::shared_ptr<Material> mat_;

  private:
    // Given some point on this sphere, translates its lat/long into [0, 1] range, i.e. u-v coordinates where the bottom
    // left is (0, 0).
    static void GetUV(const point &p, double &u, double &v);
};

} // namespace rtc

#endif