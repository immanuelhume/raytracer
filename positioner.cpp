#include "positioner.hpp"

Stationary::Stationary(point p) : p_(p) {}

// TODO this method is eating quite a bit of CPU time for something so simple
point Stationary::At(double time) { return p_; }

point Stationary::Min() { return p_; }

point Stationary::Max() { return p_; }

Parabolic::Parabolic(double period, double start, const point &low, const point &high)
    : period_(period), low_(low), dir_(high - low)
{
    a_ = -4 / (period * period);
    b_ = 4 / period;
    start_ = start - (period_ / 2);
}

point Parabolic::At(double t)
{
    t = std::fmod(t - start_, period_);
    if (t < 0) t = -t;
    return low_ + (a_ * t * t + b_ * t) * dir_;
}

point Parabolic::Min() { return low_; }

point Parabolic::Max() { return low_ + dir_; }