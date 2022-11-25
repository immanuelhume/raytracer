#ifndef TEXTURE_H
#define TEXTURE_H

#include "base.hpp"
#include "perlin.hpp"

namespace rtc
{

class Texture
{
  public:
    // Retrieve color as a function of u-v coordinates in the [0, 1] range and a point.
    virtual color ValueAt(double u, double v, const point &p) const = 0;
};

class SolidColor : public Texture
{
  public:
    SolidColor() = default;
    SolidColor(const color &c);
    SolidColor(double r, double g, double b);
    SolidColor(double r, double g, double b, double a);

    // Returns the same thing every time, it's just the color stored in here.
    virtual color ValueAt(double u, double v, const point &p) const override;

  public:
    color c_;
};

// Takes two other textures and tiles them in 3D space.
class Checkers : public Texture
{
  public:
    Checkers() = default;
    Checkers(std::shared_ptr<Texture> a, std::shared_ptr<Texture> b);
    Checkers(const color &a, const color &b);

    virtual color ValueAt(double u, double v, const point &p) const override;

  public:
    std::shared_ptr<Texture> a_;
    std::shared_ptr<Texture> b_;
};

// Perlin noise.
class Noisy : public Texture
{
  public:
    // Use perlin noise with a scaling factor. A larger factor produces "tighter" patterns. Defaults to 1.
    Noisy(double scale = 1.0);

    // Produces some color as a function the point's coordinates and the perlin noise at that point.
    virtual color ValueAt(double u, double v, const point &p) const override;

  public:
    double scale_ = 1.0; // larger for "tighter" patterns
    Perlin noise_;
};

} // namespace rtc

#endif