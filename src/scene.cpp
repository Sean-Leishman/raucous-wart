#include "scene.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "shape.hpp"

#include <any>
#include <limits>
#include <memory>
#include <vector>

Scene::Scene()
{
  bg_color = {0, 0, 0};
  shapes = std::vector<std::shared_ptr<Shape>>();
}

void Scene::add_shape(std::shared_ptr<Shape> shape)
{
  shapes.push_back(shape);
}

bool Scene::intersect(Ray ray, Intersection& return_intersection)
{
  bool hit = false;
  float hit_distance = std::numeric_limits<float>::max();

  for (const auto& object : shapes)
  {
    Intersection intersection;
    if (object->intersect(ray, 0.001f, hit_distance, intersection))
    {
      hit = true;
      if (intersection.distance < hit_distance)
      {
        hit_distance = intersection.distance;
        return_intersection = intersection;
      }
    }
  }

  return hit;
}
