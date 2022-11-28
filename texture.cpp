#include "texture.hpp"

using namespace rtc;

/* ------------------------------- BASIC COLOR ------------------------------ */

SolidColor::SolidColor(const rgb &c) : c_(c) {}
SolidColor::SolidColor(double r, double g, double b) : c_(r, g, b) {}
rgb SolidColor::ValueAt(double u, double v, const point &p) const { return c_; }

/* -------------------------------- CHECKERS -------------------------------- */

Checkers::Checkers(const rgb &a, const rgb &b)
    : a_(std::make_shared<SolidColor>(a)), b_(std::make_shared<SolidColor>(b))
{
}
Checkers::Checkers(std::shared_ptr<Texture> a, std::shared_ptr<Texture> b) : a_(a), b_(b) {}
rgb Checkers::ValueAt(double u, double v, const point &p) const
{
    double sines = glm::sin(10 * p.x) * glm::sin(10 * p.y) * glm::sin(10 * p.z); // big brain stuff
    if (sines < 0) return a_->ValueAt(u, v, p);
    else return b_->ValueAt(u, v, p);
}

/* -------------------------- VANILLA PERLIN NOISE -------------------------- */

Noisy::Noisy(double scale) : scale_(scale) {}
Noisy::Noisy(const rgb &c, double scale) : scale_(scale), c_(c) {}
rgb Noisy::ValueAt(double u, double v, const point &p) const { return c_ * 0.5 * (1.0 + noise_.Noise(scale_ * p)); }

/* ------------------------------- TURBULENCE ------------------------------- */

Turbulence::Turbulence(double scale) : Noisy(scale) {}
Turbulence::Turbulence(const rgb &c, double scale) : Noisy(c, scale) {}
rgb Turbulence::ValueAt(double u, double v, const point &p) const { return c_ * noise_.Turbulence(scale_ * p); }

/* --------------------------------- MARBLE --------------------------------- */

Marble::Marble(Axis ax, double scale) : Noisy(scale), ax_(ax) {}
Marble::Marble(Axis ax, const rgb &c, double scale) : Noisy(c, scale), ax_(ax) {}
rgb Marble::ValueAt(double u, double v, const point &p) const
{
    return c_ * 0.5 * (1 + glm::sin(scale_ * p[ax_] + 10 * noise_.Turbulence(p)));
}

/* ---------------------------------- WOOD ---------------------------------- */

Wood::Wood(double scale, double f) : Noisy(scale), f_(f) {}
Wood::Wood(const rgb &c, double scale, double f) : Noisy(c, scale), f_(f) {}
rgb Wood::ValueAt(double u, double v, const point &p) const
{
    double g = 0.5 * (1.0 + noise_.Noise(scale_ * p)) * f_;
    return (g - std::floor(g)) * c_;
}