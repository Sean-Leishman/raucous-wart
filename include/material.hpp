#pragma once

#include "image.hpp"
#include "intersection.hpp"
#include "light.hpp"
#include "ray.hpp"

#include <memory>


class Texture
{
  public:
  PPMImage image;
  bool loaded;

  Texture() : loaded(false){};
  Texture(const std::string& filename) : loaded(false)
  {
    loaded = image.read_from_file(filename);
  }

  PPMColor get_color(float u, float v) const { return image.get_pixel(u, v); }
};

class Material
{
  public:
  float ks{};
  float kd{};
  int specular_exp{};
  PPMColor diffuse_color;
  PPMColor specular_color;
  bool is_reflective{};
  float reflectivity{};
  bool is_refractive{};
  float refractive_index{};

  std::shared_ptr<Texture> texture;

  Material();
  Material(const Material&);
  Material(float, float, int, PPMColor, PPMColor, bool, float, bool, float,
           std::string);

  virtual Vec3 emission() const = 0;
  virtual bool scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation,
               Ray& scattered) const = 0;
};

class DiffuseMaterial: public Material{
  public:
      DiffuseMaterial(){};
  DiffuseMaterial(const Material& mat);
  bool scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const override;
  Vec3 emission() const override {return {};};
};

class ReflectiveMaterial: public Material{
  public:
      ReflectiveMaterial(){};
      ReflectiveMaterial(const Material& mat);
  bool scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const override;
  Vec3 emission() const override {return {};};
};

class RefractiveMaterial: public Material{
  public:
      RefractiveMaterial(){};
  RefractiveMaterial(const Material& mat);
  bool scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const override;
  Vec3 emission() const override {return {};};
};

class EmissiveMaterial: public Material {
  public:
      AreaLight light;
      EmissiveMaterial(){};
      EmissiveMaterial(const Material&, const AreaLight&);

      bool scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const override;
      Vec3 emission() const override {return light.color.to_vec();};
};
