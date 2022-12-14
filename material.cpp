#include "material.hpp"
#include <glm/gtc/random.hpp>

using namespace rtc;

/* ---------------------- LAMBERTIAN (regular diffuse) ---------------------- */
Lambertian::Lambertian(const rgb &albedo) : albedo_(std::make_shared<SolidColor>(albedo)) {}
Lambertian::Lambertian(std::shared_ptr<Texture> texture) : albedo_(texture) {}
bool Lambertian::Scatter(const Ray &ray, const HitRecord &rec, rgb &attenuation, Ray &scattered) const
{
    vec3 scatter_dir = rec.Normal() + glm::ballRand(1.0);

    if (almost_zero(scatter_dir)) // in case the scatter direction is zero, we ignore it
        scatter_dir = rec.Normal();

    scattered = Ray(rec.point_, scatter_dir, ray.time_);
    attenuation = albedo_->ValueAt(rec.u_, rec.v_, rec.point_);
    return true;
}

/* ---------------------------------- METAL --------------------------------- */
Metal::Metal(const rgb &albedo, const double fuzz) : albedo_(albedo), fuzz_(fuzz < 1 ? fuzz : 1) {}
bool Metal::Scatter(const Ray &ray, const HitRecord &rec, rgb &attenuation, Ray &scattered) const
{
    vec3 reflected = glm::reflect(ray.dir_, rec.Normal());
    scattered = Ray(rec.point_, reflected + fuzz_ * glm::ballRand(1.0), ray.time_);
    attenuation = albedo_;

    // if this dot product is positive, then the reflected ray is indeed reflected (travelling away from surface), else
    // it is absorbed
    return glm::dot(reflected, rec.Normal()) > 0;
}

/* ------------------------ DIELECTRIC (see-through) ------------------------ */
Dielectric::Dielectric(const double refractive_index) : ri_(refractive_index) {}
bool Dielectric::Scatter(const Ray &ray, const HitRecord &rec, rgb &attenuation, Ray &scattered) const
{
    double refraction_ratio = rec.FrontFace() ? (1.0 / ri_) : ri_; // assume the other material is air (1.0)
    vec3 unit_dir = glm::normalize(ray.dir_);

    // check for total internal reflection
    double cos_theta = fmin(glm::dot(-unit_dir, rec.Normal()), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    vec3 direction;

    if (cannot_refract || Reflectance(cos_theta, refraction_ratio) > rand_double())
        direction = glm::reflect(unit_dir, rec.Normal());
    else direction = glm::refract(unit_dir, rec.Normal(), refraction_ratio);

    attenuation = rgb(1, 1, 1); // a glass surface absorbs nothing, so no attenuation
    scattered = Ray(rec.point_, direction, ray.time_);

    return true;
}
double Dielectric::Reflectance(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 *= r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

/* -------------------------- DIFFUSE LIGHT SOURCE -------------------------- */
DiffuseLight::DiffuseLight(std::shared_ptr<Texture> texture) : texture_(texture) {}
DiffuseLight::DiffuseLight(const rgb &color) : texture_(std::make_shared<SolidColor>(color)) {}
bool DiffuseLight::Scatter(const Ray &ray, const HitRecord &rec, rgb &attenuation, Ray &scattered) const
{
    return false;
}
rgb DiffuseLight::Emit(double u, double v, const point &p) const { return texture_->ValueAt(u, v, p); }

/* -------------------------------- ISOTROPIC ------------------------------- */
Isotropic::Isotropic(std::shared_ptr<Texture> texture) : texture_(texture) {}
Isotropic::Isotropic(const rgb &color) : texture_(std::make_shared<SolidColor>(color)) {}
bool Isotropic::Scatter(const Ray &ray, const HitRecord &rec, rgb &attenuation, Ray &scattered) const
{
    scattered = Ray(rec.point_, glm::sphericalRand(1.0), ray.time_);
    attenuation = texture_->ValueAt(rec.u_, rec.v_, rec.point_);
    return true;
}
