#include "hittable_list.hpp"

using namespace rtc;

HittableList::HittableList()
{
}

HittableList::~HittableList()
{
}

void HittableList::Clear()
{
    objects_.clear();
}

void HittableList::Add(std::shared_ptr<Hittable> object)
{
    objects_.push_back(object);
}

bool HittableList::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    HitRecord tmp_record;
    bool hit_something = false;
    double best_t = tmax;

    for (const auto &object : objects_)
    {
        if (object->Hit(ray, tmin, best_t, tmp_record))
        {
            hit_something = true;
            best_t = tmp_record.t_;
            record = tmp_record;
        }
    }

    return hit_something;
}
