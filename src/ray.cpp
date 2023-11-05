#include "ray.hpp"

Vec3 Ray::point_at_parameter(float dist) const
{
  return origin + direction * dist;
}