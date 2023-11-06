#pragma once

#include "vector.hpp"

class Ray
{
  public:
  Vec3 origin;
  Vec3 direction;

  Ray()= default;

  Vec3 point_at_parameter(float dist) const;
  Vec3 point_at_parameter(Vec3 origin, Vec3 dir, float dist) const;
};

