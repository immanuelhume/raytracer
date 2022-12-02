#include "box.hpp"
#include "aarect.hpp"

using namespace rtc;

Box::Box(const point &min, const point &max, std::shared_ptr<Material> mat) : min_(min), max_(max)
{
    sides_.Add(std::make_shared<XYRect>(min.x, max.x, min.y, max.y, min.z, mat));
    sides_.Add(std::make_shared<XYRect>(min.x, max.x, min.y, max.y, max.z, mat));

    sides_.Add(std::make_shared<XZRect>(min.x, max.x, min.z, max.z, min.y, mat));
    sides_.Add(std::make_shared<XZRect>(min.x, max.x, min.z, max.z, max.y, mat));

    sides_.Add(std::make_shared<YZRect>(min.y, max.y, min.z, max.y, min.x, mat));
    sides_.Add(std::make_shared<YZRect>(min.y, max.y, min.z, max.y, max.x, mat));
}

bool Box::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    return sides_.Hit(ray, tmin, tmax, record);
}

bool Box::BoundingBox(double t0, double t1, AABB &box) const
{
    box.min_ = min_;
    box.max_ = max_;
    return true;
}