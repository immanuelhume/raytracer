#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "hittable.hpp"
#include "texture.hpp"

namespace rtc
{

class ConstantMedium : public Hittable
{
  public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, std::shared_ptr<Texture> texture, double density);
    ConstantMedium(std::shared_ptr<Hittable> boundary, const rgb &color, double density);

    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

  public:
    std::shared_ptr<Hittable> boundary_;
    std::shared_ptr<Material> mat_;
    double neg_inv_density_; // cached division
};

} // namespace rtc

#endif