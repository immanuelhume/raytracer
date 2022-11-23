#include "sphere.hpp"

using namespace rtc;

Sphere::Sphere(double radius, std::shared_ptr<Positioner> position, std::shared_ptr<Material> mat)
    : radius_(radius), position_(position), mat_(mat)
{
}

Sphere::~Sphere()
{
}

bool Sphere::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    // compute the sphere's center at time defined by the ray
    point center = position_->At(ray.time_);

    vec3 oc = ray.origin_ - center;
    double a = glm::dot(ray.dir_, ray.dir_);
    double half_b = glm::dot(oc, ray.dir_);
    double c = glm::dot(oc, oc) - radius_ * radius_;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return false;

    double sqrtd = sqrt(discriminant);
    double root = (-half_b - sqrtd) / a; // the closer root
    if (root > tmax)
        return false;

    if (root < tmin)
    {
        root = (-half_b + sqrtd) / a; // the larger root
        if (root < tmin)
            return false;
    }

    record.t_ = root;
    record.point_ = ray.At(root);
    record.set_face_normal(ray, (record.point_ - center) / radius_);
    record.mat_ = mat_;

    return true;
}

bool Sphere::BoundingBox(double t0, double t1, AABB &box) const
{
    box.min_ = position_->Min() - vec3(radius_, radius_, radius_);
    box.max_ = position_->Max() - vec3(radius_, radius_, radius_);
    return true;
}