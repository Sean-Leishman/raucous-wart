#pragma once

#include "image.hpp"

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

      Material();
      Material(float, float, int, PPMColor, PPMColor, bool, float, bool, float);
};