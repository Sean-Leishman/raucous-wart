#pragma once

#include "image.hpp"

class Texture
{
  public:
  //     PPMImage image;
  bool loaded;

  Texture() : loaded(false){};

  PPMColor get_color(float u, float v) const
  {
    // return image.get_pixel(u, v);
    return PPMColor();
  }
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

  Texture texture;

  Material();
  Material(float, float, int, PPMColor, PPMColor, bool, float, bool, float,
           std::string);
};
