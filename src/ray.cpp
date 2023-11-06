#include "ray.hpp"

Vec3 Ray::point_at_parameter(float dist) const
{
  return origin + direction * dist;
}

Vec3 Ray::point_at_parameter(Vec3 origin, Vec3 dir, float dist) const
{
  return origin + dir * dist;
}