#ifndef AARECT_H
#define AARECT_H

#include "hittable.hpp"
#include "material.hpp"

namespace rtc
{

// rectangle along x-y plane
class XYRect : public Hittable
{
  public:
    XYRect(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> mat);
    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

  public:
    double x0_, x1_, y0_, y1_; // the four lines which draw the rectangle
    double k_;                 // position along z axis
    std::shared_ptr<Material> mat_;
};

// rectangle along x-z plane
class XZRect : public Hittable
{
  public:
    XZRect(double x0, double x1, double z0, double z1, double k, std::shared_ptr<Material> mat);
    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

  public:
    double x0_, x1_, z0_, z1_; // the four lines which draw the rectangle
    double k_;                 // position along z axis
    std::shared_ptr<Material> mat_;
};

// rectangle along y-z plane
class YZRect : public Hittable
{
  public:
    YZRect(double y0, double y1, double z0, double z1, double k, std::shared_ptr<Material> mat);
    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const override;
    virtual bool BoundingBox(double t0, double t1, AABB &box) const override;

  public:
    double y0_, y1_, z0_, z1_; // the four lines which draw the rectangle
    double k_;                 // position along z axis
    std::shared_ptr<Material> mat_;
};

} // namespace rtc

#endif