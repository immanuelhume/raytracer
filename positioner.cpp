#include "positioner.hpp"

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