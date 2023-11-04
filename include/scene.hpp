#pragma once

#include "intersection.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vector.hpp"

#include <memory>
#include <vector>

struct BackgroundColor
{
  float r;
  float g;
  float b;
};

class Scene
{
  private:
  BackgroundColor bg_color;

  public:
  std::vector<std::shared_ptr<Shape>> shapes;

  Scene();
  void add_shape(std::shared_ptr<Shape> shape);

  bool intersect(Ray ray, Intersection& hit_info);
};
