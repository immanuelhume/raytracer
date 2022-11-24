#ifndef TEXTURE_H
#define TEXTURE_H

#include "base.hpp"
#include "perlin.hpp"

namespace rtc
{

class Texture
{
  public:
    virtual color ValueAt(double u, double v, const point &p) const = 0;
};

class SolidColor : public Texture
{
  public:
    SolidColor() = default;
    SolidColor(const color &c);
    SolidColor(double r, double g, double b);

    virtual color ValueAt(double u, double v, const point &p) const override;

  public:
    color c_;
};

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

class Noisy : public Texture
{
  public:
    Noisy() = default;
    // Construct a noisy texture with a scaling factor. A larger factor produces "tighter" patterns.
    Noisy(double scale);
    virtual color ValueAt(double u, double v, const point &p) const override;

  public:
    double scale_ = 1.0; // larger for "tighter" patterns
    Perlin noise_;
};

} // namespace rtc

#endif