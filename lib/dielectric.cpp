#include "dielectric.hpp"
#include "base.hpp"

using namespace rtc;

Dielectric::Dielectric()
{
}

Dielectric::Dielectric(const double refractive_index) : ri_(refractive_index)
{
}

Dielectric::~Dielectric()
{
}

bool Dielectric::scatter(const Ray &ray, const HitRecord &rec, color &attenuation, Ray &scattered) const
{
    double refraction_ratio = rec.front_face() ? (1.0 / ri_) : ri_; // assume the other material is air (1.0)
    vec3 unit_dir = glm::normalize(ray.dir_);

    // check for total internal reflection
    double cos_theta = fmin(glm::dot(-unit_dir, rec.normal()), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > rand_double())
        direction = glm::reflect(unit_dir, rec.normal());
    else
        direction = glm::refract(unit_dir, rec.normal(), refraction_ratio);

    attenuation = color(1, 1, 1, 1); // a glass surface absorbs nothing, so no attenuation
    scattered = Ray(rec.point_, direction);

    return true;
}

// schlick approximation
double Dielectric::reflectance(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 *= r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}
