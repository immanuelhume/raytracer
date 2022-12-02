#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "hittable.hpp"

namespace rtc
{

class Translate : public Hittable
{
  public:
    Translate(std::shared_ptr<Hittable> obj, const vec3 &offset);
    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

  public:
    std::shared_ptr<Hittable> obj_;
    vec3 offset_;
};

class Rotate : public Hittable
{
  public:
    // rotate about some axis by some radians
    Rotate(std::shared_ptr<Hittable> obj, Axis ax, double rad);
    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

  public:
    std::shared_ptr<Hittable> obj_;
    std::function<vec3(const vec3 &)> rotate_;
    std::function<vec3(const vec3 &)> rotate_back_; // the reverse transformation

    double sin_theta_;
    double cos_theta_;

    bool has_box_;
    AABB bbox_;
};

} // namespace rtc

#endif