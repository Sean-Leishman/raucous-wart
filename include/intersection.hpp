#pragma once

#include "input.hpp"
#include "shape.hpp"
#include "vector.hpp"

#include "memory"

enum IntersectionType
{
  NORMAL,
  CAPS,
};

class Shape;

class Intersection
{
  public:
  Vec3 normal;
  Vec3 position;
  float distance{};
  IntersectionType type;
  std::shared_ptr<const Shape> object;

  Intersection() : type(NORMAL){};
};
