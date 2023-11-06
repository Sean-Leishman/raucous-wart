#pragma once

#include "input.hpp"
#include "shape.hpp"
#include "vector.hpp"

#include "memory"

class Shape;

class Intersection
{
  public:
      Vec3 normal;
  Vec3 position;
  float distance{};
  std::shared_ptr<const Shape> object;

  Intersection()= default;
};
