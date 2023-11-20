#include "material.hpp"
#include "image.hpp"
#include "vector.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "random.h"

#include <memory>

Material::Material()
    : ks(1.0f), kd(1.0f), specular_exp(1), diffuse_color(PPMColor()),
      specular_color(PPMColor()), is_reflective(false), is_refractive(true),
      reflectivity(1.0f), refractive_index(1.0f),
      texture(std::make_shared<Texture>())
{}

Material::Material(const Material& m): ks(m.ks), kd(m.kd), specular_exp(m.specular_exp), diffuse_color(m.diffuse_color),
                                      specular_color(m.specular_color), is_reflective(m.is_reflective), is_refractive(m.is_refractive),
                                      reflectivity(m.reflectivity), refractive_index(m.refractive_index),
                                      texture(m.texture){}

DiffuseMaterial::DiffuseMaterial(const Material& mat): Material(mat){}
bool DiffuseMaterial::scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const
{
  Vec3 scatter_dir = hit_info.normal + random_unit_vector();
  scattered = {hit_info.position, scatter_dir};
  attenuation = diffuse_color.to_vec();
  return true;
}

ReflectiveMaterial::ReflectiveMaterial(const Material& mat): Material(mat){}
bool ReflectiveMaterial::scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const
{
  Vec3 scatter_dir = hit_info.normal + random_unit_vector();
  scattered = {hit_info.position, scatter_dir};
  attenuation = diffuse_color.to_vec();
  return true;
}

RefractiveMaterial::RefractiveMaterial(const Material& mat): Material(mat){}
bool RefractiveMaterial::scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const
{
  Vec3 scatter_dir = hit_info.normal + random_unit_vector();
  scattered = {hit_info.position, scatter_dir};
  attenuation = diffuse_color.to_vec();
  return true;
}
