#ifndef POSITIONER_H
#define POSITIONER_H

#include "base.hpp"

// in essence encapsulates some kind of function defined over real numbers
class Positioner
{
  public:
    virtual point At(double time) = 0;
    // gives the max x, y, z coordinates possible
    virtual point Max() = 0;
    // gives the min x, y, z coordinates possible
    virtual point Min() = 0;
};

// Can represent a stationary object.
class Stationary : public Positioner
{
  public:
    Stationary(point p);
    virtual point At(double time) override;
    virtual point Max() override;
    virtual point Min() override;

  public:
    point p_;
};

class Parabolic : public Positioner
{
  public:
    Parabolic(double period, double start = 0, point low = point(0, 0, 0), point high = point(0, 1, 0));
    virtual point At(double time) override;
    virtual point Max() override;
    virtual point Min() override;

  private:
    double a_, b_;
    double start_;
    double period_;
    point low_;
    vec3 dir_;
};

#endif