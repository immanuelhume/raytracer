#include "constant_medium.hpp"
#include "material.hpp"

using namespace rtc;

ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, std::shared_ptr<Texture> texture, double density)
    : boundary_(boundary), mat_(std::make_shared<Isotropic>(texture)), neg_inv_density_(-1.0 / density)
{
}

ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, const rgb &color, double density)
    : boundary_(boundary), mat_(std::make_shared<Isotropic>(color)), neg_inv_density_(-1.0 / density)
{
}

bool ConstantMedium::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    HitRecord rec1, rec2;
    if (!boundary_->Hit(ray, ninfinity, infinity, rec1)) return false;
    if (!boundary_->Hit(ray, rec1.t_ + 0.001, infinity, rec2)) return false;

    // clamp the times
    rec1.t_ = std::max(rec1.t_, tmin);
    rec2.t_ = std::min(rec2.t_, tmax);

    if (rec1.t_ >= rec2.t_) return false; // sanity check?
    if (rec1.t_ < 0) return false;        // ditto?

    double ray_length = glm::length(ray.dir_);
    double dist_inside_boundary = (rec2.t_ - rec1.t_) * ray_length;
    double hit_distance = neg_inv_density_ * std::log(rand_double());

    if (hit_distance > dist_inside_boundary) return false;

    record.t_ = rec1.t_ + hit_distance / ray_length;
    record.point_ = ray.At(record.t_);
    record.mat_ = mat_;

    // record.SetFaceNormal(ray, vec3(0, 1, 0));

    return true;
}

bool ConstantMedium::BoundingBox(double t0, double t1, AABB &box) const { return boundary_->BoundingBox(t0, t1, box); }