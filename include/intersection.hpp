#pragma once

#include "input.hpp"
#include "shape.hpp"
#include "vector.hpp"

struct Intersection
{
  Vec3 normal;
  Vec3 position;
  float distance;
  std::shared_ptr<Shape> object;
};
