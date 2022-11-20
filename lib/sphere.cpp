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

Stationary::Stationary(point p) : p_(p)
{
}

point Stationary::At(double time)
{
    return p_;
}

Parabolic::Parabolic(double period, double start, point low, point high) : period_(period), low_(low), dir_(high - low)
{
    a_ = -4 / (period * period);
    b_ = 4 / period;
    start_ = start - (period_ / 2);
}

point Parabolic::At(double t)
{
    t = std::fmod(t - start_, period_);
    if (t < 0)
        t = -t;
    return low_ + (a_ * t * t + b_ * t) * dir_;
}