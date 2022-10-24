#include "lambertian.hpp"
#include "base.hpp"
#include <glm/gtc/random.hpp>

using namespace rtc;

Lambertian::Lambertian()
{
}

Lambertian::Lambertian(const color &albedo) : albedo_(albedo)
{
}

Lambertian::~Lambertian()
{
}

bool Lambertian::scatter(const Ray &ray, const HitRecord &rec, color &attenuation, Ray &scattered) const
{
    vec3 scatter_dir = rec.normal() + glm::ballRand(1.0);

    if (almost_zero(scatter_dir)) // in case the scatter_dir is bad
        scatter_dir = rec.normal();

    scattered = Ray(rec.point_, scatter_dir);
    attenuation = albedo_;
    return true;
}
