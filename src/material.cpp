#include "material.hpp"
#include "image.hpp"
#include "intersection.hpp"
#include "light.hpp"
#include "random.h"
#include "ray.hpp"
#include "vector.hpp"

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
  scatter_dir.normalize();

  scattered = {hit_info.position, scatter_dir};
  attenuation = diffuse_color.to_vec() * kd;
  return true;
}

ReflectiveMaterial::ReflectiveMaterial(const Material& mat): Material(mat){}
bool ReflectiveMaterial::scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const
{
  attenuation = (specular_color * reflectivity).to_vec();
  float ir = refractive_index;

  Vec3 unit_dir = Vec3::normalize(ray.direction);
  double cos_theta = fmin((unit_dir * -1).dot(hit_info.normal), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

  bool cannot_refract = ir * sin_theta > 1.0;

  auto r0 = (1 - ir) / (1+ir);
  r0 *= r0;
  r0 = r0 + (1-r0)*pow((1-cos_theta),5);
  // r0 > random_double()

  Vec3 dir;
  if (is_reflective) {
    if (r0 > random_double()) {
      dir = Vec3::reflect(unit_dir, hit_info.normal);
    }
  }
  if (is_refractive){
    if (!cannot_refract){
      dir = Vec3::refract(unit_dir, hit_info.normal, ir);
    }
  }
  if (dir.x == 0 && dir.y == 0 && dir.z == 0){
    return false;
  }
  unit_dir = Vec3::normalize(dir);
  scattered = Ray(hit_info.position + unit_dir * 0.000001f, unit_dir);
  return true;
}

RefractiveMaterial::RefractiveMaterial(const Material& mat): Material(mat){}
bool RefractiveMaterial::scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const
{
  Vec3 scatter_dir = hit_info.normal + random_unit_vector();
  scatter_dir.normalize();

  scattered = {hit_info.position, scatter_dir};
  attenuation = diffuse_color.to_vec();
  return true;
}

EmissiveMaterial::EmissiveMaterial(const Material& mat, const AreaLight& light): Material(mat), light(light){}
bool EmissiveMaterial::scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const
{
  attenuation = light.intensity;
 return false;
}

