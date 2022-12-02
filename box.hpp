#ifndef BOX_H
#define BOX_H

#include "hittable.hpp"
#include "hittable_list.hpp"

namespace rtc
{

class Box : public Hittable
{
  public:
    Box(const point &min, const point &max, std::shared_ptr<Material> mat);
    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

  public:
    point min_;
    point max_;
    HittableList sides_;
};

} // namespace rtc

#endif