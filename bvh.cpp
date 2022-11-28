#include "bvh.hpp"

using namespace rtc;

std::function<bool(std::shared_ptr<Hittable>, std::shared_ptr<Hittable>)> BoxCompare(Axis axis)
{
    return [axis](const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        AABB box_a, box_b;

        if (!a->BoundingBox(0, 0, box_a) || !a->BoundingBox(0, 0, box_b))
            return true; // there's no meaningful comparison possible, just return true

        return box_a.min_[axis] < box_b.min_[axis];
    };
}

BVHNode::BVHNode(const HittableList &list, double t0, double t1)
    : BVHNode(list.objects_, 0, list.objects_.size(), t0, t1)
{
}

BVHNode::BVHNode(const std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end, double t0, double t1)
{
    auto xs = objects; // make a copy!
    auto comparator = BoxCompare(rand_axis());

    // Check how many items are in this subtree. If its one or two, then we have a leaf node. Otherwise we'll need to
    // recursively construct new left and right BVH nodes.
    size_t n = end - start;
    switch (n)
    {
    case 0: return;
    case 1:
        left_ = right_ = xs[start]; // why is it in both left and right?
        break;
    case 2:
        if (!comparator(xs[start], xs[start + 1])) std::swap(xs[start], xs[start + 1]);
        left_ = xs[start];
        right_ = xs[start + 1];
        break;
    default:
        std::sort(xs.begin() + start, xs.begin() + end, comparator);
        size_t mid = start + n / 2;

        left_ = std::make_shared<BVHNode>(xs, start, mid, t0, t1);
        right_ = std::make_shared<BVHNode>(xs, mid, end, t0, t1);
    }

    // finally, make a box for this node
    AABB box_left, box_right;
    if (!left_->BoundingBox(t0, t1, box_left) || !right_->BoundingBox(t0, t1, box_right))
        std::cerr << "BVHNode: could not get bounding box when constructing\n";

    box_ = AABB::Surround(box_left, box_right);
}

bool BVHNode::Hit(const Ray &ray, double tmin, double tmax, HitRecord &record) const
{
    if (!box_.Hit(ray, tmin, tmax)) return false;

    // We'll hit the left or right or both. But we only want the hit with the smallest t.
    bool hit_left = left_->Hit(ray, tmin, tmax, record);
    bool hit_right = right_->Hit(ray, tmin, hit_left ? record.t_ : tmax, record);
    return hit_left || hit_right;
}

bool BVHNode::BoundingBox(double t0, double t1, AABB &box) const
{
    box = box_;
    return true;
}
