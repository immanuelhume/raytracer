#ifndef MATERIAL_H
#define MATERIAL_H

#include "base.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "texture.hpp"

namespace rtc
{

class Material
{
  public:
    Material() = default;
    virtual ~Material() = default;
    // Tries to compute a scattered ray from from intersection point, and returns true if such a scattered ray was
    // produced.
    virtual bool scatter(const Ray &ray, const HitRecord &rec, rgb &attenuation, Ray &scattered) const = 0;
};

// a diffuse material
class Lambertian : public Material
{
  public:
    Lambertian(const rgb &albedo);
    Lambertian(std::shared_ptr<Texture> texture);
    virtual ~Lambertian() override = default;
    virtual bool scatter(const Ray &ray, const HitRecord &rec, rgb &attenuation, Ray &scattered) const override;

  public:
    std::shared_ptr<Texture> albedo_ = nullptr;
};

class Metal : public Material
{
  public:
    Metal() = default;
    // create a metalic material, where albedo is the tint and fuzz in [0, 1]
    Metal(const rgb &albedo, double fuzz);
    virtual ~Metal() override = default;
    virtual bool scatter(const Ray &ray, const HitRecord &rec, rgb &attenuation, Ray &scattered) const override;

  public:
    rgb albedo_ = rgb(1.0, 1.0, 1.0);

    // degree of randomness in direction of scattered ray, 0 for a mirror-like look and 1 for an almost diffuse look
    double fuzz_ = 0.0;
};

class Dielectric : public Material
{
  public:
    Dielectric() = default;
    Dielectric(double refractive_index);
    virtual ~Dielectric() override = default;
    virtual bool scatter(const Ray &ray, const HitRecord &rec, rgb &attenuation, Ray &scattered) const override;

  private:
    // shlick approximation
    static double reflectance(double cosine, double ref_idx);

  public:
    double ri_ = 1.0; // the refractive index
};

} // namespace rtc

#endif