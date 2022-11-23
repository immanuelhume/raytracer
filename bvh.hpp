#ifndef BVH_H
#define BVH_H

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

namespace rtc
{

// A tree of hittables. Only the leaves are "primitive" objects, every internal node is an actual BVHNode.
class BVHNode : public Hittable
{
  public:
    BVHNode() = default;
    BVHNode(const HittableList &list, double t0, double t1);
    BVHNode(const std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end, double t0, double t1);

    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

  public:
    std::shared_ptr<Hittable> left_ = nullptr;
    std::shared_ptr<Hittable> right_ = nullptr;
    AABB box_;
};

} // namespace rtc

#endif