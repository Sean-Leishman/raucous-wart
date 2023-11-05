#pragma once

#include "intersection.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vector.hpp"

#include <memory>
#include <vector>


class Scene
{
  public:
      PPMColor bg_color;
  std::vector<std::shared_ptr<Shape>> shapes;

  Scene();
  void add_shape(std::shared_ptr<Shape> shape);

  bool intersect(Ray ray, Intersection& hit_info);
};
