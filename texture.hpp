#ifndef TEXTURE_H
#define TEXTURE_H

#include "base.hpp"
#include "perlin.hpp"

namespace rtc
{

class Texture
{
  public:
    virtual ~Texture() = default;
    // Retrieve color as a function of 1) u-v coordinates in the [0, 1] range and 2) a point.
    virtual rgb ValueAt(double u, double v, const point &p) const = 0;
};

class SolidColor : public Texture
{
  public:
    SolidColor() = default;
    SolidColor(const rgb &c);
    SolidColor(double r, double g, double b);

    // Returns the same thing every time, it's just the color stored in here.
    virtual rgb ValueAt(double u, double v, const point &p) const override;

  public:
    rgb c_;
};

// Takes two other textures and tiles them in 3D space.
class Checkers : public Texture
{
  public:
    Checkers() = default;
    Checkers(std::shared_ptr<Texture> a, std::shared_ptr<Texture> b);
    Checkers(const rgb &a, const rgb &b);

    virtual rgb ValueAt(double u, double v, const point &p) const override;

  public:
    std::shared_ptr<Texture> a_;
    std::shared_ptr<Texture> b_;
};

// Just perlin noise.
class Noisy : public Texture
{
  public:
    // Use perlin noise with a scaling factor. A larger factor produces "tighter" patterns. Defaults to 1.
    Noisy(double scale = 1.0);
    Noisy(const rgb &c, double scale = 1.0);
    virtual ~Noisy() = default;

    // Produces some color as a function the point's coordinates and the perlin noise at that point.
    virtual rgb ValueAt(double u, double v, const point &p) const override;

  public:
    double scale_ = 1.0; // larger for "tighter" patterns
    Perlin noise_;
    rgb c_ = rgb(1.0, 1.0, 1.0); // black by default
};

class Turbulence : public Noisy
{
  public:
    Turbulence(double scale = 1.0);
    Turbulence(const rgb &c, double scale = 1.0);
    virtual rgb ValueAt(double u, double v, const point &p) const override;
};

class Marble : public Noisy
{
  public:
    // Choose an axis for the marble's bands to lie on.
    Marble(Axis ax, double scale = 1.0);
    Marble(Axis ax, const rgb &c, double scale = 1.0);
    virtual rgb ValueAt(double u, double v, const point &p) const override;

  public:
    Axis ax_;
};

class Wood : public Noisy
{
  public:
    Wood(double scale = 1.0, double f = 8.0);
    Wood(const rgb &c, double scale = 1.0, double f = 8.0);
    virtual rgb ValueAt(double u, double v, const point &p) const override;

  public:
    double f_;
};

} // namespace rtc

#endif