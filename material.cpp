#include "material.hpp"
#include <glm/gtc/random.hpp>

using namespace rtc;

Lambertian::Lambertian() {}

Lambertian::Lambertian(const color &albedo) : albedo_(std::make_shared<SolidColor>(albedo)) {}

Lambertian::Lambertian(std::shared_ptr<Texture> texture) : albedo_(texture) {}

Lambertian::~Lambertian() {}

bool Lambertian::scatter(const Ray &ray, const HitRecord &rec, color &attenuation, Ray &scattered) const
{
    vec3 scatter_dir = rec.normal() + glm::ballRand(1.0);

    if (almost_zero(scatter_dir)) // in case the scatter direction is zero, we ignore it
        scatter_dir = rec.normal();

    scattered = Ray(rec.point_, scatter_dir, ray.time_);
    attenuation = albedo_->ValueAt(rec.u_, rec.v_, rec.point_);
    return true;
}

Metal::Metal() {}

Metal::Metal(const color &albedo, const double fuzz) : albedo_(albedo), fuzz_(fuzz < 1 ? fuzz : 1) {}

Metal::~Metal() {}

bool Metal::scatter(const Ray &ray, const HitRecord &rec, color &attenuation, Ray &scattered) const
{
    vec3 reflected = glm::reflect(ray.dir_, rec.normal());
    scattered = Ray(rec.point_, reflected + fuzz_ * glm::ballRand(1.0), ray.time_);
    attenuation = albedo_;

    // if this dot product is positive, then the reflected ray is indeed reflected (travelling away from surface), else
    // it is absorbed
    return glm::dot(reflected, rec.normal()) > 0;
}

Dielectric::Dielectric() {}

Dielectric::Dielectric(const double refractive_index) : ri_(refractive_index) {}

Dielectric::~Dielectric() {}

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
    else direction = glm::refract(unit_dir, rec.normal(), refraction_ratio);

    attenuation = color(1, 1, 1, 1); // a glass surface absorbs nothing, so no attenuation
    scattered = Ray(rec.point_, direction, ray.time_);

    return true;
}

// schlick approximation
double Dielectric::reflectance(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 *= r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}
