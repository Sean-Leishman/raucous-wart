#include "material.hpp"
#include "image.hpp"
#include "vector.hpp"

#include <memory>

Material::Material()
    : ks(1.0f), kd(1.0f), specular_exp(1), diffuse_color(PPMColor()),
      specular_color(PPMColor()), is_reflective(false), is_refractive(true),
      reflectivity(1.0f), refractive_index(1.0f),
      texture(std::make_shared<Texture>(Texture()))
{
}
