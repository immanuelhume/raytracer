#include "sphere.hpp"
#include <glm/gtx/norm.hpp>

using namespace rtc;

Sphere::Sphere()
{
}

Sphere::Sphere(point center, double radius, std::shared_ptr<Material> mat) : center_(center), radius_(radius), mat_(mat)
{
}

Sphere::~Sphere()
{
}

bool Sphere::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    vec3 oc = ray.origin_ - center_;
    double a = glm::length2(ray.dir_);
    double half_b = glm::dot(oc, ray.dir_);
    double c = glm::length2(oc) - radius_ * radius_;
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
    record.set_face_normal(ray, (record.point_ - center_) / radius_);
    record.mat_ = mat_;

    return true;
}
