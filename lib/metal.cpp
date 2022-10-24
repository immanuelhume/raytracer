#include "metal.hpp"
#include <glm/gtc/random.hpp>

using namespace rtc;

Metal::Metal()
{
}

Metal::Metal(const color &albedo, const double fuzz) : albedo_(albedo), fuzz_(fuzz < 1 ? fuzz : 1)
{
}

Metal::~Metal()
{
}

bool Metal::scatter(const Ray &ray, const HitRecord &rec, color &attenuation, Ray &scattered) const
{
    vec3 reflected = glm::reflect(ray.dir_, rec.normal());
    scattered = Ray(rec.point_, reflected + fuzz_ * glm::ballRand(1.0));
    attenuation = albedo_;

    // if this dot product is positive, then the reflected ray is indeed reflected (travelling away from surface), else
    // it is absorbed
    return glm::dot(reflected, rec.normal()) > 0;
}