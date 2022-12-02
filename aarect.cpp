#include "aarect.hpp"

using namespace rtc;

XYRect::XYRect(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> mat)
    : x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), mat_(mat)
{
}

bool XYRect::BoundingBox(double t0, double t1, AABB &box) const
{
    box.min_ = point(x0_, y0_, k_ - 0.0001);
    box.max_ = point(x1_, y1_, k_ + 0.0001);
    return true;
}

bool XYRect::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    double t = (k_ - ray.origin_.z) / ray.dir_.z;
    if (t < tmin || t > tmax) return false;

    point i = ray.origin_ + t * ray.dir_; // intersection point
    if (i.x < x0_ || i.x > x1_ || i.y < y0_ || i.y > y1_) return false;

    record.t_ = t;
    record.u_ = (i.x - x0_) / (x1_ - x0_);
    record.v_ = (i.y - y0_) / (y1_ - y0_);
    record.mat_ = mat_;
    record.point_ = i;
    record.set_face_normal(ray, vec3(0, 0, 1));

    return true;
}

XZRect::XZRect(double x0, double x1, double z0, double z1, double k, std::shared_ptr<Material> mat)
    : x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), mat_(mat)
{
}

bool XZRect::BoundingBox(double t0, double t1, AABB &box) const
{
    box.min_ = point(x0_, k_ - 0.0001, z0_);
    box.max_ = point(x1_, k_ + 0.0001, z1_);
    return true;
}

bool XZRect::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    double t = (k_ - ray.origin_.y) / ray.dir_.y;
    if (t < tmin || t > tmax) return false;

    point i = ray.origin_ + t * ray.dir_; // intersection point
    if (i.x < x0_ || i.x > x1_ || i.z < z0_ || i.z > z1_) return false;

    record.t_ = t;
    record.u_ = (i.x - x0_) / (x1_ - x0_);
    record.v_ = (i.y - z0_) / (z1_ - z0_);
    record.mat_ = mat_;
    record.point_ = i;
    record.set_face_normal(ray, vec3(0, 1, 0));

    return true;
}

YZRect::YZRect(double y0, double y1, double z0, double z1, double k, std::shared_ptr<Material> mat)
    : y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k), mat_(mat)
{
}

bool YZRect::BoundingBox(double t0, double t1, AABB &box) const
{
    box.min_ = point(k_ - 0.0001, y0_, z0_);
    box.max_ = point(k_ + 0.0001, y1_, z1_);
    return true;
}

bool YZRect::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    double t = (k_ - ray.origin_.x) / ray.dir_.x;
    if (t < tmin || t > tmax) return false;

    point i = ray.origin_ + t * ray.dir_; // intersection point
    if (i.y < y0_ || i.y > y1_ || i.z < z0_ || i.z > z1_) return false;

    record.t_ = t;
    record.u_ = (i.y - y0_) / (y1_ - y0_);
    record.v_ = (i.y - z0_) / (z1_ - z0_);
    record.mat_ = mat_;
    record.point_ = i;
    record.set_face_normal(ray, vec3(1, 0, 0));

    return true;
}