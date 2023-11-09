#pragma once

#include "intersection.hpp"
#include "light.hpp"
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
  std::vector<std::shared_ptr<Light>> lights;

  std::shared_ptr<Light> ambient_light;

  Scene();
  void add_shape(std::shared_ptr<Shape> shape);

  bool intersect(Ray ray, Intersection& hit_info);
  bool object_in_shadow(Ray, std::shared_ptr<const Shape>, Intersection*);
};
