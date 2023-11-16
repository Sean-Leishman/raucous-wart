#pragma once

#include "image.hpp"

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
  float ks;
  float kd;
  int specular_exp;
  PPMColor diffuse_color;
  PPMColor specular_color;
  bool is_reflective;
  float reflectivity;
  bool is_refractive;
  float refractive_index;

  std::shared_ptr<Texture> texture;

  Material();
  Material(float, float, int, PPMColor, PPMColor, bool, float, bool, float,
           std::string);
};
