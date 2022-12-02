#ifndef HITTABLE_H
#define HITTABLE_H

#include "aabb.hpp"
#include "base.hpp"
#include "ray.hpp"

namespace rtc
{

class Material; // just to avoid circular reference

// a hit for some ray
class HitRecord
{
  public:
    HitRecord() = default;

    void SetFaceNormal(const Ray &ray, const vec3 &outward_normal)
    {
        front_face_ = glm::dot(ray.dir_, outward_normal) < 0.0;
        normal_ = front_face_ ? outward_normal : -outward_normal;
    }

    const vec3 &Normal() const { return normal_; };

    // returns true if the ray is incident to surface from the outside
    bool FrontFace() const { return front_face_; }

  public:
    point point_;
    double t_;
    double u_; // u coordinate of surface, in [0, 1]
    double v_; // v coorindate of surface, in [0, 1]
    std::shared_ptr<Material> mat_;

  private:
    vec3 normal_;     // always points towards the ray
    bool front_face_; // true if the ray intersects from outside
};

class Hittable
{
  public:
    Hittable() = default;
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const = 0;
    // Constructs a bounding box for this object. Returns false if impossible (e.g. if object is a plane).
    // TODO: make all implementors cache this
    virtual bool BoundingBox(double t0, double t1, AABB &box) const = 0;
};

} // namespace rtc

#endif