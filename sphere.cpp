#include "sphere.hpp"

using namespace rtc;

Sphere::Sphere(double radius, const point &center, std::shared_ptr<Material> mat)
    : radius_(radius), position_(std::make_shared<Stationary>(center)), mat_(mat)
{
}

Sphere::Sphere(double radius, std::shared_ptr<Positioner> position, std::shared_ptr<Material> mat)
    : radius_(radius), position_(position), mat_(mat)
{
}

bool Sphere::Hit(const Ray &ray, double tmin, double tmax, HitRecord &rec) const
{
    // compute the sphere's center at time defined by the ray
    point center = position_->At(ray.time_);

    vec3 oc = ray.origin_ - center;
    double a = glm::dot(ray.dir_, ray.dir_);
    double half_b = glm::dot(oc, ray.dir_);
    double c = glm::dot(oc, oc) - radius_ * radius_;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return false;

    double sqrtd = sqrt(discriminant);
    double root = (-half_b - sqrtd) / a; // the closer root
    if (root > tmax) return false;

    if (root < tmin)
    {
        root = (-half_b + sqrtd) / a; // the larger root
        if (root < tmin) return false;
    }

    rec.t_ = root;
    rec.point_ = ray.At(root);
    rec.mat_ = mat_;

    vec3 outward_normal = (rec.point_ - center) / radius_;
    rec.set_face_normal(ray, outward_normal);
    GetUV(outward_normal, rec.u_, rec.v_);

    return true;
}

bool Sphere::BoundingBox(double t0, double t1, AABB &box) const
{
    box.min_ = position_->Min() - vec3(radius_, radius_, radius_);
    box.max_ = position_->Max() + vec3(radius_, radius_, radius_);
    return true;
}

void Sphere::GetUV(const point &p, double &u, double &v)
{
    // Imagine p as a vector from the center to the surface. Let theta be the angle between -y and p, and phi be the
    // angle between -x and p. Then u and v are just theta and phi scaled to [0, 1] respectively. This gives us a pair
    // of 2D coordinates where (0, 0) represents the bottom left.
    double theta = std::acos(-p.y);
    double phi = std::atan2(-p.z, p.x) + M_PI;

    u = phi / (2 * M_PI);
    v = theta / M_PI;
}