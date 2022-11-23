#include "texture.hpp"

using namespace rtc;

SolidColor::SolidColor(const color &c) : c_(c){};
SolidColor::SolidColor(double r, double g, double b) : c_(r, g, b, 1){};
color SolidColor::ValueAt(double u, double v, const point &p) const
{
    return c_;
}

Checkers::Checkers(const color &a, const color &b)
    : a_(std::make_shared<SolidColor>(a)), b_(std::make_shared<SolidColor>(b))
{
}
Checkers::Checkers(std::shared_ptr<Texture> a, std::shared_ptr<Texture> b) : a_(a), b_(b)
{
}
color Checkers::ValueAt(double u, double v, const point &p) const
{
    double sines = glm::sin(10 * p.x) * glm::sin(10 * p.y) * glm::sin(10 * p.z); // big brain stuff
    if (sines < 0)
        return a_->ValueAt(u, v, p);
    else
        return b_->ValueAt(u, v, p);
}