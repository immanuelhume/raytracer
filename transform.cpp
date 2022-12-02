#include "transform.hpp"

using namespace rtc;

Translate::Translate(std::shared_ptr<Hittable> obj, const vec3 &offset) : obj_(obj), offset_(offset) {}

bool Translate::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    Ray shifted(ray.origin_ - offset_, ray.dir_, ray.time_);
    if (!obj_->Hit(shifted, tmin, tmax, record)) return false;

    record.point_ += offset_;
    record.SetFaceNormal(shifted, record.Normal());

    return true;
}

bool Translate::BoundingBox(double t0, double t1, AABB &box) const
{
    if (!obj_->BoundingBox(t0, t1, box)) return false;

    box.min_ += offset_;
    box.max_ += offset_;

    return true;
}

Rotate::Rotate(std::shared_ptr<Hittable> obj, Axis ax, double rad)
    : obj_(obj), sin_theta_(glm::sin(rad)), cos_theta_(glm::cos(rad))
{
    // store a lambda for all rotations
    switch (ax)
    {
    case Axis::x:
        rotate_ = [this](const vec3 &v) {
            double newy = cos_theta_ * v.y - sin_theta_ * v.y;
            double newz = sin_theta_ * v.y + cos_theta_ * v.z;
            return vec3(v.x, newy, newz);
        };
        rotate_back_ = [this](const vec3 &v) {
            double newy = cos_theta_ * v.y + sin_theta_ * v.y;
            double newz = -sin_theta_ * v.y + cos_theta_ * v.z;
            return vec3(v.x, newy, newz);
        };
        break;
    case Axis::y:
        rotate_ = [this](const vec3 &v) {
            double newx = cos_theta_ * v.x + sin_theta_ * v.z;
            double newz = -sin_theta_ * v.x + cos_theta_ * v.z;
            return vec3(newx, v.y, newz);
        };
        rotate_back_ = [this](const vec3 &v) {
            double newx = cos_theta_ * v.x - sin_theta_ * v.z;
            double newz = sin_theta_ * v.x + cos_theta_ * v.z;
            return vec3(newx, v.y, newz);
        };
        break;
    case Axis::z:
        rotate_ = [this](const vec3 &v) {
            double newx = cos_theta_ * v.x - sin_theta_ * v.y;
            double newy = sin_theta_ * v.x + cos_theta_ * v.y;
            return vec3(newx, newy, v.z);
        };
        rotate_back_ = [this](const vec3 &v) {
            double newx = cos_theta_ * v.x + sin_theta_ * v.y;
            double newy = -sin_theta_ * v.x + cos_theta_ * v.y;
            return vec3(newx, newy, v.z);
        };
        break;
    default: std::cerr << "received unknown axis: " << ax << std::endl; exit(1); // just die
    }

    point min(infinity, infinity, infinity);
    point max(ninfinity, ninfinity, ninfinity);

    has_box_ = obj_->BoundingBox(0.0, 1.0, bbox_);
    if (!has_box_) return;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                double x = i * bbox_.min_.x + (1 - i) * bbox_.max_.x;
                double y = j * bbox_.min_.y + (1 - j) * bbox_.max_.y;
                double z = k * bbox_.min_.z + (1 - k) * bbox_.max_.z;

                point corner(x, y, z);
                corner = rotate_(corner);

                for (int c = 0; c < 3; c++)
                {
                    min[c] = std::min(min[c], corner[c]);
                    max[c] = std::max(max[c], corner[c]);
                }
            }

    bbox_ = AABB(min, max);
}

bool Rotate::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    Ray rotated_ray(rotate_back_(ray.origin_), rotate_back_(ray.dir_));

    if (!obj_->Hit(rotated_ray, tmin, tmax, record)) return false;

    record.point_ = rotate_(record.point_);
    record.SetFaceNormal(rotated_ray, rotate_(record.Normal()));

    return true;
}

bool Rotate::BoundingBox(double t0, double t1, AABB &box) const
{
    box = bbox_;
    return has_box_;
}