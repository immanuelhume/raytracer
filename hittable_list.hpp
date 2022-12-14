#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.hpp"

namespace rtc
{

class HittableList : public Hittable
{
  public:
    HittableList();
    ~HittableList() override;
    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

    void Clear(); // clears all objects in this list
    void Add(std::shared_ptr<Hittable> object);

  public:
    std::vector<std::shared_ptr<Hittable>> objects_;
};

} // namespace rtc

#endif