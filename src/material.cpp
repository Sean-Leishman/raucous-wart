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
      texture(std::make_shared<Texture>(Texture()))
{
}


bool Material::scatter(Ray& ray, Intersection& hit_info, Vec3& attenuation, Ray& scattered) const
{
  Vec3 scatter_dir = hit_info.normal + random_unit_vector();
  scattered = {hit_info.position, scatter_dir};
  attenuation = diffuse_color.to_vec();
  return true;
}